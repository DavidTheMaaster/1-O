#include "j1Menu.h"
#include "p2Defs.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Window.h"


j1Menu::j1Menu()
{
	name.create("menu");
}

j1Menu::~j1Menu()
{
}

bool j1Menu::Awake(pugi::xml_node &config)
{
	bool ret = true;

	resolutions_file.load_file(config.child("resolutions").attribute("folder").as_string());
	resolution_node = resolutions_file.child("resolutions").first_child();

	while(resolution_node !=NULL)
	{
		int animation_id = resolution_node.child("data").attribute("id").as_int();
		
		if (animation_id == 0)
		{
			load_animation = &res_0;
		}
		if (animation_id == 1)
		{
			load_animation = &res_1;
		}
		if (animation_id == 2)
		{
			load_animation = &res_2;
		}
		if (animation_id == 3)
		{
			load_animation = &res_3;
		}
		if (animation_id == 4)
		{
			load_animation = &res_4;
		}

		SDL_Rect r;
		r.x = resolution_node.child("animation").attribute("x").as_int();
		r.y = resolution_node.child("animation").attribute("y").as_int();
		r.w = resolution_node.child("animation").attribute("w").as_int();
		r.h = resolution_node.child("animation").attribute("h").as_int();

		load_animation->PushBack({r.x,r.y,r.w,r.h});

		resolution_node = resolution_node.next_sibling();
	}

	resolution_node = resolutions_file.child("resolutions");

	return ret;
}

bool j1Menu::Start()
{
	bool ret = true;
	resolution = 0;
	resolution_textures = App->tex->Load("textures/resolutions.png");
	return ret;
}

bool j1Menu::CleanUp()
{
	bool ret = true;

	return ret;
}

bool j1Menu::Update(float dt)
{
	bool ret = true;

	if (App->scene->level == menu) 
	{
		mouse = MousePosition();
		ret = ActionMenu();
	}
	else if (App->scene->level == options)
	{
		mouse = MousePosition();
		ActionOptions();
		Resolution();
		Draw();
	}
	return ret;
}

iPoint j1Menu::MousePosition()
{
	iPoint vec;
	App->input->GetMousePosition(vec.x, vec.y);
	vec = App->map->WorldToMap(vec.x, vec.y);

	return iPoint(vec);
}

bool j1Menu::ActionMenu()
{
	bool ret = true;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
	{
		if (App->map->logic_layer->data->Get(mouse.x, mouse.y) == PLAY) {
			App->scene->level = level_1;
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1);
		}
		else if (App->map->logic_layer->data->Get(mouse.x, mouse.y) == OPTIONS) {
			App->scene->level = options;
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1);
		}
		else if (App->map->logic_layer->data->Get(mouse.x, mouse.y) == EXIT)
		{
			ret = false;
		}
	}
	return ret;
}

void j1Menu::ActionOptions()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (App->map->logic_layer->data->Get(mouse.x, mouse.y) == left)
		{
			if (resolution > 0)
			{
				resolution--;
			}
			else
			{
				resolution = 4;
			}
		}
		if (App->map->logic_layer->data->Get(mouse.x, mouse.y) == right)
		{
			if (resolution < 4)
			{
				resolution++;
			}
			else
			{
				resolution = 0;
			}
		}
		if (App->map->logic_layer->data->Get(mouse.x, mouse.y) == exit)
		{
			App->scene->level = menu;
			SetResolution(window_size.x, window_size.y);
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 0.5f);
		}
	}
}

void j1Menu::Resolution()
{
	
	if (resolution == 0)
	{
		current_animation = &res_0;
		window_size.x = resolution_node.child("res_0").child("data").attribute("width").as_int();
		window_size.y = resolution_node.child("res_0").child("data").attribute("height").as_int();
	}
	if (resolution == 1)
	{
		current_animation = &res_1;
		window_size.x = resolution_node.child("res_1").child("data").attribute("width").as_int();
		window_size.y = resolution_node.child("res_1").child("data").attribute("height").as_int();
	}
	if (resolution == 2)
	{
		current_animation = &res_2;
		window_size.x = resolution_node.child("res_2").child("data").attribute("width").as_int();
		window_size.y = resolution_node.child("res_2").child("data").attribute("height").as_int();
	}
	if (resolution == 3)
	{
		current_animation = &res_3;
		window_size.x = resolution_node.child("res_3").child("data").attribute("width").as_int();
		window_size.y = resolution_node.child("res_3").child("data").attribute("height").as_int();
	}
	if (resolution == 4)
	{
		current_animation = &res_4;
		window_size.x = resolution_node.child("res_4").child("data").attribute("width").as_int();
		window_size.y = resolution_node.child("res_4").child("data").attribute("height").as_int();
	}
}

void j1Menu::Draw()
{
	if (App->fadetoblack->IsFading()== false)
	{
		App->render->Blit(resolution_textures, 540, 197, &(current_animation->GetCurrentFrame()));
	}
}

void j1Menu::SetResolution(uint x, uint y)
{
	App->win->SetWindowSize(x, y);
}
