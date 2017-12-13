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

	cross.PushBack({1,1,56,59});
	cross.PushBack({ 57,1,56,59 });
	cross.PushBack({ 113,1,56,59 });
	cross.PushBack({ 169,1,56,59 });
	cross.PushBack({ 225,1,56,59 });
	cross.PushBack({ 281,1,56,59 });
	cross.PushBack({ 337,1,56,59 });
	cross.PushBack({ 393,1,56,59 });
	cross.PushBack({ 449,1,56,59 });
	cross.PushBack({ 1,60,56,59 });
	cross.PushBack({ 57,60,56,59 });
	cross.PushBack({ 113,60,56,59 });
	cross.PushBack({ 169,60,56,59 });
	cross.PushBack({ 225,60,56,59 });
	cross.PushBack({ 281,60,56,59 });
	cross.PushBack({ 337,60,56,59 });
	cross.PushBack({ 393,60,56,59 });
	cross.PushBack({ 449,60,56,59 });
	cross.PushBack({ 1,119,56,59 });
	cross.PushBack({ 57,119,56,59 });
	cross.PushBack({ 113,119,56,59 });
	cross.PushBack({ 169,119,56,59 });
	cross.PushBack({ 225,119,56,59 });
	cross.PushBack({ 281,119,56,59 });
	cross.PushBack({ 337,119,56,59 });
	cross.PushBack({ 393,119,56,59 });
	cross.PushBack({ 449,119,56,59 });
	cross.PushBack({ 1,178,56,59 });
	cross.PushBack({ 57,178,56,59 });

	cross.speed = 0.5f;
	cross.loop = false;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{	
	menu_texture = App->tex->Load("maps/menu.png");
	buttons = App->tex->Load("maps/menu_button.png");
	cross_texture = App->tex->Load("maps/cross.png");
	hand_texture = App->tex->Load("maps/hand.png");

	if (level == menu)
	{
		App->gui->AddImage(0, 0, menu_texture);
		play = App->gui->AddButton(537, 368, buttons,this);
		options = App->gui->AddButton(650, 368, buttons, this);
		exit = App->gui->AddButton(764, 368, buttons, this);
		App->gui->AddLabel(537, 305,"PLAY",BLACK, UPHEAVAL,20);
		App->gui->AddLabel(530, 335, "JOGAR", BLACK, UPHEAVAL, 20);
		App->gui->AddLabel(635, 305, "OPTIONS", BLACK, UPHEAVAL, 20);
		App->gui->AddLabel(630, 335, "D'OPCIONS", BLACK, UPHEAVAL, 20);
		App->gui->AddLabel(768, 305, "EXIT", BLACK, UPHEAVAL, 20);
		App->gui->AddLabel(758, 335, "SORTIR", BLACK, UPHEAVAL, 20);
		hand = App->gui->AddImage(337, 420, hand_texture, {});
	}

	if (level == level_1) {
		App->map->Load("level1.tmx");
		App->audio->PlayMusic("audio/music/all_of_us.ogg");

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
	bool ret = true;
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
	}
	*/
	
	switch (level)
	{
	case menu:
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
				App->gui->AddImage(0, 0, cross_texture, cross, play);
				hand->pos.x = 1000;
				hand = App->gui->AddImage(337, 420, hand_texture, {});
				play_ui = true;
				change = true;
			}
			if (options->state == L_MOUSE_PRESSED)
			{
				App->gui->AddImage(0, 0, cross_texture, cross, options);
				hand->pos.x = 1000;
				hand = App->gui->AddImage(450, 420, hand_texture, {});
			}


			if (exit->state == L_MOUSE_PRESSED)
			{
				App->gui->AddImage(0, 0, cross_texture, cross, exit);
				hand->pos.x = 1000;
				hand = App->gui->AddImage(563, 420, hand_texture, {});
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
			if (exit_ui)
				ret = false;
		}

		break;
	default:
		break;
	}

	
	
	
	App->map->Draw();
	CheckChange();

	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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

bool j1Scene::Animations()
{
	bool ret = false;
	if (change == true)
	{
		HandAnimation();
		if (i == 30)
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


