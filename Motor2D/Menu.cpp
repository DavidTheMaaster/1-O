#include "Menu.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1App.h"


Menu::Menu()
{
	name.create("menu");
}

Menu::~Menu()
{
}

inline bool Menu::Start()
{
	ui_texture = App->tex->Load("maps/UI.png");
	menu_texture = App->tex->Load("maps/menu.png");
	cross_texture = App->tex->Load("maps/cross.png");
	cross_click_fx = App->audio->LoadFx("audio/fx/pencil_lines.wav");
	button_focused_fx = App->audio->LoadFx("audio/fx/mouse_over.wav");
	return true;
}
