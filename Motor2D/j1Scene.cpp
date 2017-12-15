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
	bool ret = true;

	level_name = config.child("levels");

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{	
	level_change_fx = App->audio->LoadFx("audio/fx/change_level.wav");
	ui_texture = App->tex->Load("maps/UI.png");
	if (level == MENU)
	{
		App->menu->LoadMenuUI();
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
	bool ret = App->menu->exit_game;

	GetKeys();
	App->map->Draw();
	CheckChange();
	ButtonInteractions();

	if (App->pause)
		App->render->DrawQuad({ 0,0,1920,1080 }, 0, 0, 0, 175, true, false);

	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (level != MENU && level != OPTIONS && level != MENU_PLAY)
	{
		Pause();
	}
	if (level == OPTIONS)
	{
		App->render->DrawQuad({ 575 + App->audio->volume*2, 235, 2, 34 }, 255, 100, 100, 255);
	}
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	App->audio->UnLoadFx(level_change_fx);
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
	default:
		break;
	}
}



void j1Scene::LoadLevelUI()
{

}

void j1Scene::Pause()
{
	/*if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		App->pause = !App->pause;
		if (App->pause == true)
		{
			sheet = App->gui->AddImage(490, 35, ui_texture, sheet_anim);
		}
		if (App->pause == false && sheet != nullptr)
		{
			App->gui->DeleteUI(sheet);
		}
	}*/
}