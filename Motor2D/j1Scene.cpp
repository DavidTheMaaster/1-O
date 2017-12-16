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

	if (level == MENU)
	{
		App->menu->LoadMenuUI();
	}

	if (level == level_1) {
		LoadLevelUI();
		start_time = SDL_GetTicks();
		App->map->Load("level1.tmx");
		App->audio->PlayMusic("audio/music/level_music.ogg");

	}
	if (level == level_2) {
		LoadLevelUI();
		App->map->Load("level2.tmx");
	}
	if (level == hidden_level) {
		LoadLevelUI();
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
	Timer();

	if (App->paused)
		App->render->DrawQuad({ 0,0,1920,1080 }, 0, 0, 0, 175, true, false);

	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

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
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
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
	App->gui->AddImage(900, 10, ui_texture, lifes_anim);
	App->gui->AddImage(50, 10, ui_texture, urn_anim);
	App->gui->AddImage(900, 490, ui_texture, ammo_anim);
	App->gui->AddLabel(865, 28, "x3", BLACK, MINECRAFT, 25);
}

void j1Scene::Timer()
{
	total_time = 300;
	uint currentTime = SDL_GetTicks();

	currentTime = currentTime - start_time;

	total_time = 300 - MiliToSeconds(currentTime);

	LOG("%i %i %i", start_time, currentTime, total_time);

}

uint j1Scene::MiliToSeconds(uint time)
{
	return time / 1000;
}