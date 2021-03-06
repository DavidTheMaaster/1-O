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
#include "Menu.h"
#include "Pause.h"

#include <stdio.h> 
 

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

	level_name = config.child("levels");


	animation_file.load_file("animations.xml");
	animations = animation_file.child("animations").child("ui").child("ingame").first_child();

	if (animations == NULL)
	{
		LOG("Could not load animations");
	}


	while (animations != NULL) {
		attributes = animations.child("attributes");
		rect = animations.first_child();

		current = attributes.attribute("id").as_int();

		if (current == LIFES)
			load_anim = &lifes_anim;
		if (current == URN_UI)
			load_anim = &urn_anim;
		if (current == AMMO)
			load_anim = &ammo_anim;

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

		animations = animations.next_sibling();

	}
	return true;
}

// Called before the first frame
bool j1Scene::Start()
{	
	
	level_change_fx = App->audio->LoadFx("audio/fx/change_level.wav");
	ui_texture = App->tex->Load("maps/UI.png");
	lose_texture = App->tex->Load("maps/lose_screen.png");
	

	if (level == MENU)
	{
		App->menu->Start();
		App->menu->ResetMenu();
		App->menu->LoadMenuUI();
	}

	if (level == level_1) {
		last_lvl_score = 0;
		score = last_lvl_score;
		urns = 0;
		player_lifes = 5;
		LoadLevelUI();
		App->pause->Start();
		if (timer_start)
		{
			start_time = SDL_GetTicks();
			timer_start = false;
			tmp_time = 0;
		}
		App->map->Load("level1.tmx");
		App->audio->PlayMusic("audio/music/level_music.ogg");

	}
	if (level == level_2) {
		score = last_lvl_score;
		urns = 0;
		LoadLevelUI();
		if (timer_start)
		{
			start_time = SDL_GetTicks();
			timer_start = false;
			tmp_time = 0;
		}
		App->map->Load("level2.tmx");
	}
	if (level == hidden_level) {
		score = last_lvl_score;
		urns = 0;
		LoadLevelUI();
		if (timer_start)
		{
			start_time = SDL_GetTicks();
			timer_start = false;
			tmp_time = 0;
		}
		App->map->Load("hidden_level.tmx");
	}


	changeMap = false;

	App->map->SetMapLogic();

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	if (level == congrats || level == LOSE) {
		if (level == LOSE)
		{
			highscore = score;
			LoadLoseScreen();
			App->audio->PlayMusic("audio/music/lose_music.ogg");
		}
		else if (level == congrats) {
			highscore = score;
			LoadHighScore();
			App->audio->PlayMusic("audio/music/win_music.ogg");
			App->map->Load("levelwin.tmx");
			App->render->camera.x = 0; App->render->camera.y = 0;
			App->entities->congrats = false;
		}
	}
	else if(level != level_1 && level != level_2 || level != hidden_level) {
		App->entities->Start();
	}
	ammo = 10;

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
	bool ret = App->menu->exit_game;

	GetKeys();
	App->map->Draw();
	CheckChange();
	ButtonInteractions();
	Timer();
	
	if(level == level_1 || level == level_2 || level == hidden_level)
		UpdateLevelUI();

	if (App->paused)
		App->render->DrawQuad({ 0,0,1920,1080 }, 0, 0, 0, 175, true, false);

	if (player_lifes == 0)
	{
		level = LOSE;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1);
		player_lifes = 5;
	}
	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	UnLoadLevelUI();
	App->audio->UnLoadFx(level_change_fx);
	App->tex->UnLoad(ui_texture);
	App->tex->UnLoad(lose_texture);
	App->map->DeleteMap();
	App->map->CleanUp();
	App->collision->CleanUp();
	App->entities->CleanUp();
	App->menu->CleanUp();
	App->pause->CleanUp();
	App->gui->CleanUp();
	return true;
}

void j1Scene::CheckChange()
{
	if (changeMap) {
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 2);
		App->audio->PlayFx(level_change_fx);
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
	if (data.child("score") == NULL) {
		data.append_child("score").append_attribute("score") = score;
	}
	else {
		data.append_child("score").attribute("score") = score;
	}
	if (data.child("ammo") == NULL) {
		data.append_child("ammo").append_attribute("ammo") = ammo;
	}
	else {
		data.append_child("ammo").attribute("ammo") = ammo;
	}
	if (data.child("player_lifes") == NULL) {
		data.append_child("player_lifes").append_attribute("player_lifes") = player_lifes;
	}
	else {
		data.append_child("player_lifes").attribute("player_lifes") = player_lifes;
	}
	if (data.child("urns") == NULL) {
		data.append_child("urns").append_attribute("urns") = urns;
	}
	else {
		data.append_child("urns").attribute("urns") = urns;
	}
	if (data.child("time") == NULL) {
		data.append_child("time").append_attribute("time") = urns;
	}
	else {
		data.append_child("time").attribute("time") = urns;
	}

	ret = true;
	return ret;
}

// Load
bool j1Scene::Load(pugi::xml_node& data) {

	if (data.child("level") != NULL) {
		if (data.child("level").attribute("level").as_int() != level) {
			level = data.child("level").attribute("level").as_int();
			score = data.child("score").attribute("score").as_int();
			ammo = data.child("ammo").attribute("ammo").as_int();
			player_lifes = data.child("player_lifes").attribute("player_lifes").as_int();
			urns = data.child("urns").attribute("urns").as_int();
			time = data.child("time").attribute("time").as_int();
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 2);
			justloaded = true;
		}
		
		else {
			score = data.child("score").attribute("score").as_int();
			ammo = data.child("ammo").attribute("ammo").as_int();
			player_lifes = data.child("player_lifes").attribute("player_lifes").as_int();
			urns = data.child("urns").attribute("urns").as_int();
			time = data.child("time").attribute("time").as_int();
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
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 4);
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		Start();
	}
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		CleanUp();
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}
	if (level == LOSE || level == congrats)
	{
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			level = MENU;
			App->menu->ResetMenu();
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
		}
	}

	
}

void j1Scene::ButtonInteractions()
{
	switch (level)
	{
	case MENU:
		App->menu->MenuButtons();
		break;
	case OPTIONS:
		App->menu->OptionButtons();
		break;
	case MENU_PLAY:
		App->menu->PlayButtons();
		break;
	case MENU_CREDIT:
		App->menu->CreditButton();
		break;
	default:
		break;
	}
}



void j1Scene::LoadLevelUI()
{
	life_image = App->gui->AddImage(900, 10, ui_texture, lifes_anim);
	urn_image = App->gui->AddImage(50, 10, ui_texture, urn_anim);
	ammo_image = App->gui->AddImage(900, 490, ui_texture, ammo_anim);
	lifes_ui = App->gui->AddLabel(880, 20, "0", BLACK, MINECRAFT, 30);
	urn_ui = App->gui->AddLabel(100, 20, "0", BLACK, MINECRAFT, 30);
	ammo_ui = App->gui->AddLabel(860, 500, "10", BLACK, MINECRAFT, 30);
	timer_ui = App->gui->AddLabel(450, 20, "100", BLACK, UPHEAVAL, 50);
	score_ui = App->gui->AddLabel(880, 60, "200", BLACK, UPHEAVAL, 30);
	UpdateLevelUI();
}

void j1Scene::UnLoadLevelUI()
{
	App->gui->DeleteUI(urn_ui);
	App->gui->DeleteUI(lifes_ui);
	App->gui->DeleteUI(ammo_ui);
	App->gui->DeleteUI(timer_ui);
	App->gui->DeleteUI(score_ui);
	App->gui->DeleteUI(ammo_image);
	App->gui->DeleteUI(urn_image);
	App->gui->DeleteUI(life_image);


}

void j1Scene::UpdateLevelUI()
{
	
	if (lifes_ui != nullptr)
	{
		std::string s = std::to_string(player_lifes);
		p2SString lifes_label = s.c_str();
		if (player_lifes < 2) {
			lifes_ui->ChangeLabel(lifes_label.GetString(), RED);
		}
		else
			lifes_ui->ChangeLabel(lifes_label.GetString(), BLACK);

		std::string s2 = std::to_string(urns);
		p2SString urn_label = s2.c_str();
		if (urns == 3) {
			urn_ui->ChangeLabel(urn_label.GetString(), YELLOW);
		}
		else
			urn_ui->ChangeLabel(urn_label.GetString(), BLACK);

		std::string s3 = std::to_string(ammo);
		p2SString ammo_label = s3.c_str();
		if (ammo <= 3) {
			ammo_ui->ChangeLabel(ammo_label.GetString(), RED);
		}
		else 
			ammo_ui->ChangeLabel(ammo_label.GetString(), BLACK);

		std::string s4 = std::to_string(total_time);
		p2SString time_label = s4.c_str();
		if (total_time >= 120) {
			timer_ui->ChangeLabel(time_label.GetString(), RED);
		}
		else 
			timer_ui->ChangeLabel(time_label.GetString(), BLACK);

		if (score < 0)
			score = 0;
		std::string s5 = std::to_string(score);
		p2SString score_label = s5.c_str();
		score_ui->ChangeLabel(score_label.GetString(), BLACK);

	}
}

void j1Scene::LoadLoseScreen()
{
	lose_image = App->gui->AddImage(0,0,lose_texture);
}

void j1Scene::LoadHighScore()
{
	if (highscore > highscore2)
	{
		highscore2 = highscore;
	}
	if (level == congrats)
	{
		std::string s = std::to_string(highscore2);
		p2SString highscore_string = s.c_str();
		highscore_label = App->gui->AddLabel(75, 320,"HIGHSCORE:", BLACK,FREEPIXEL,50);
		highscore_number = App->gui->AddLabel(335, 320, (char*)highscore_string.GetString(), BLACK, FREEPIXEL, 50);
	}

	if (level == MENU && highscore2 > 0)
	{
		std::string s = std::to_string(highscore2);
		p2SString highscore_string = s.c_str();
		highscore_label = App->gui->AddLabel(240,200, "HIGHSCORE:", WHITE, FREEPIXEL, 40);
		highscore_number = App->gui->AddLabel(320,250, (char*)highscore_string.GetString(), WHITE, FREEPIXEL, 50);
	}
}


void j1Scene::Timer()
{
	total_time = 0;
	uint currentTime = SDL_GetTicks();

	currentTime = currentTime - start_time;

	total_time = MiliToSeconds(currentTime);

	if (total_time >= 120) {
		if (tmp_time != total_time) {
			score -= 5;
		}
		tmp_time = total_time;
	}


	LOG("%i %i %i", start_time, currentTime, total_time);

}

uint j1Scene::MiliToSeconds(uint time)
{
	return time / 1000;
}
