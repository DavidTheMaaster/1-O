#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Entities.h"
#include "j1Collision.h"
#include "j1PathFinding.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1Window.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	level_name = config.child("levels");


	animation_file.load_file("animations.xml");
	animations = animation_file.child("animations").child("ui").first_child();

	if (animations == NULL)
	{
		LOG("Could not load animations");
	}

	while (animations != NULL) {
		attributes = animations.child("attributes");
		rect = animations.first_child();

		current = attributes.attribute("id").as_int();

		if (current == CROSS)
		{
			load_anim = &cross_anim;
		}

		int i = rect.attribute("id").as_int();
		int j = attributes.attribute("size").as_int();

		while (i < j)
		{
			SDL_Rect r;
			r.x = rect.attribute("x").as_int();
			r.y = rect.attribute("y").as_int();
			r.w = rect.attribute("w").as_int();
			r.h = rect.attribute("h").as_int();

			load_anim->PushBack({ r.x,r.y,r.w,r.h });

			rect = rect.next_sibling();
			i = rect.attribute("id").as_int();
			load_anim->loop = attributes.attribute("loop").as_bool();
			load_anim->speed = attributes.attribute("speed").as_float();

		}

		anim_speed = load_anim->speed;

		animations = animations.next_sibling();

	}


	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{	
	menu_texture = App->tex->Load("maps/menu.png");
	buttons = App->tex->Load("maps/menu_button.png");
	cross_texture = App->tex->Load("maps/cross.png");
	hand_texture = App->tex->Load("maps/hand.png");
	sheet_text = App->tex->Load("maps/sheet.png");
	option_sheet_text = App->tex->Load("maps/option_sheet.png");
	exit_options_text = App->tex->Load("maps/exit_button.png");

	if (level == MENU)
	{
		App->gui->AddImage(0, 0, menu_texture);
		sheet = App->gui->AddImage(490,35,sheet_text);
		play = App->gui->AddButton(47, 333, buttons,this, sheet);
		options = App->gui->AddButton(160, 333, buttons, this, sheet);
		exit = App->gui->AddButton(274, 333, buttons, this, sheet);
		App->gui->AddLabel(47, 275,"PLAY",BLACK, UPHEAVAL,20, sheet);
		App->gui->AddLabel(40, 305, "JOGAR", BLACK, UPHEAVAL, 20, sheet);
		App->gui->AddLabel(145, 275, "OPTIONS", BLACK, UPHEAVAL, 20, sheet);
		App->gui->AddLabel(140, 305, "D'OPCIONS", BLACK, UPHEAVAL, 20, sheet);
		App->gui->AddLabel(278, 275, "EXIT", BLACK, UPHEAVAL, 20, sheet);
		App->gui->AddLabel(268, 305, "SORTIR", BLACK, UPHEAVAL, 20, sheet);
		hand = App->gui->AddImage(337, 420, hand_texture, {});
	}

	if (level == level_1) {
		App->map->Load("level1.tmx");
		App->audio->PlayMusic("audio/music/level_music.ogg");

	}
	if (level == level_2) {
		App->map->Load("level2.tmx");
	}
	if (level == hidden_level) {
		App->map->Load("hidden_level.tmx");
	}
	

	changeMap = false;

	App->map->SetMapLogic();
	//App->player->Start();

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	if (level == congrats) {
		App->map->Load("levelwin.tmx");
		App->render->camera.x = 0; App->render->camera.y = 0;
	}
	else {
		App->entities->Start();
	}


	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	exit_game = true;
	UpdateSpeed(dt);



	GetKeys();
	SetUI();
	
	App->map->Draw();
	CheckChange();


	if (App->pause)
	{
		
			App->render->DrawQuad({ 0,0,1920,1080}, 0, 0, 0, 100,true,false);
	}

	return exit_game;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (level != MENU && level != OPTIONS)
	{
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			App->pause = !App->pause;
			if (App->pause == true)
			{
				sheet = App->gui->AddImage(490, 35, sheet_text);
			}
			if (App->pause == false && sheet != nullptr)
			{
				App->gui->DeleteUI(sheet);
			}
		}
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->map->DeleteMap();
	App->map->CleanUp();
	App->collision->CleanUp();
	App->entities->CleanUp();
	App->gui->CleanUp();
	return true;
}

void j1Scene::CheckChange()
{
	if (changeMap) {
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1);
	}
}

bool j1Scene::Save(pugi::xml_node& data) const {
	bool ret = false;

	if (data.child("level") == NULL) {
		data.append_child("level").append_attribute("level") = level;
	}
	else {
		data.append_child("level").attribute("level") = level;
	}

	ret = true;
	return ret;
}

// Load
bool j1Scene::Load(pugi::xml_node& data) {

	if (data.child("level") != NULL) {
		if (data.child("level").attribute("level").as_int() != level) {
			level = data.child("level").attribute("level").as_int();
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
			justloaded = true;
		}
		
		else {
			justloaded = true;
		}
		
	}

	return true;
}

void j1Scene::GetKeys()
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		level = hidden_level;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
		level = level_2;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		level = level_1;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}
	/*
	if (player_lifes == 0){
	level = GameOverScreen;
	App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}*/
}

void j1Scene::SetUI()
{
	switch (level)
	{
	case MENU:
		if (change == false)
		{
			if (play->state == MOUSE_ENTER)
				hand->pos.x = 337;
			if (options->state == MOUSE_ENTER)
				hand->pos.x = 450;
			if (exit->state == MOUSE_ENTER)
				hand->pos.x = 563;


			if (play->state == L_MOUSE_PRESSED)
			{
				cross = App->gui->AddImage(0, 0, cross_texture, cross_anim, play);
				App->gui->DeleteUI(hand);
				hand = App->gui->AddImage(337, 420, hand_texture, {});
				play_ui = true;
				change = true;
			}
			if (options->state == L_MOUSE_PRESSED)
			{
				cross = App->gui->AddImage(0, 0, cross_texture, cross_anim, options);
				App->gui->DeleteUI(hand);
				hand = App->gui->AddImage(450, 420, hand_texture, {});
				options_ui = true;
				change = true;
			}

			if (exit->state == L_MOUSE_PRESSED)
			{
				cross = App->gui->AddImage(0, 0, cross_texture, cross_anim, exit);
				App->gui->DeleteUI(hand);
				hand = App->gui->AddImage(563, 420, hand_texture);
				exit_ui = true;
				change = true;
			}
		}

		if (Animations())
		{
			if (play_ui)
			{
				level = level_1;
				App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
			}
			if (options_ui)
			{
				option_sheet = App->gui->AddImage(490, 25, option_sheet_text);
				exit_options = App->gui->AddButton(275,50,exit_options_text, this, option_sheet);
				morevolume = App->gui->AddButton(100, 200, buttons, this, option_sheet);
				lessvolume = App->gui->AddButton(200, 200, buttons, this, option_sheet);

				level = OPTIONS;
			}
			if (exit_ui)
				exit_game = false;
		}

		break;
	case OPTIONS:
		if (exit_options->state == L_MOUSE_PRESSED)
		{
			App->gui->DeleteUI(exit_options);
			App->gui->DeleteUI(option_sheet);
			App->gui->DeleteUI(morevolume);
			App->gui->DeleteUI(lessvolume);
			ResetMenu();
		}
		if (morevolume->state == L_MOUSE_PRESSED)
		{
			
		}
		if (lessvolume->state == L_MOUSE_PRESSED)
		{
			
		}
		break;
	default:
		break;
	}
}

bool j1Scene::Animations()
{
	bool ret = false;
	if (change == true)
	{
		HandAnimation();
		if (i >= 30)
		{
			change = false;
			ret = true;
		}
		else
		{
			i++;
		}
	}
	return ret;
}

void j1Scene::HandAnimation()
{

	if (move == true)
	{
		hand->pos.y -= 10;
		if (hand->pos.y == 350)
		{
			move = false;
			move2 = true;
		}
	}
	if(move2 == true)
	{
		hand->pos.y += 5;
		hand->pos.x += 5;
		if (hand->pos.y == 400)
		{
			move2 = false;
			move3 = true;
		}
	}
	if (move3 == true)
	{
		hand->pos.y -= 10;
		if (hand->pos.y == 350)
		{
			move3 = false;
			move4 = true;
		}
	}
	if (move4 == true)
	{
		hand->pos.y += 5;
		hand->pos.x -= 5;
		if (hand->pos.y == 400)
		{
			move4 = false;
		}
	}
}

void j1Scene::ResetMenu()
{
	i = 0;
	move = true;
	cross_anim.Reset();
	App->gui->DeleteUI(cross);
	hand->pos.x = 337;
	hand->pos.y = 420;
	play_ui = options_ui = exit_ui = false;
	level = MENU;
}


void j1Scene::UpdateSpeed(float dt)
{
	cross_anim.speed = anim_speed * dt;
}