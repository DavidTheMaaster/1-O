#include "j1Audio.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1Input.h"
#include "Pause.h"
#include "j1FadeToBlack.h"


Pause::Pause()
{
	name.create("pause");
}

Pause::~Pause()
{
}

bool Pause::Awake(pugi::xml_node & node)
{
	animation_file.load_file("animations.xml");
	animations = animation_file.child("animations").child("pause").first_child();

	if (animations == NULL)
	{
		LOG("Could not load animations");
	}



	while (animations != NULL) {
		attributes = animations.child("attributes");
		rect = animations.first_child();

		current = attributes.attribute("id").as_int();

		if (current == BUTTON_PAUSE)
			load_anim = &button_anim;
		if (current == BUTTON_PAUSE_FOCUSED)
			load_anim = &button_focused_anim;
		if (current == SHEET_PAUSE)
			load_anim = &sheet_anim;

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

bool Pause::Start()
{
	ui_texture = App->tex->Load("maps/UI.png");
	//17
	return true;
}

bool Pause::Update(float dt)
{
	if (App->scene->level != MENU && App->scene->level != OPTIONS && App->scene->level != MENU_PLAY)
	{
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			App->paused = !App->paused;
			if (App->paused)
				LoadPause();
			else
				UnloadPause();
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		App->scene->level = MENU;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}
	if (App->paused)
		PauseButtons();

return true;
}

bool Pause::CleanUp()
{
	App->tex->UnLoad(ui_texture);
	return true;
}

void Pause::LoadPause()
{
	sheet = App->gui->AddImage(490, 35, ui_texture, sheet_anim);
	resume_button = App->gui->AddButton(17, 50, ui_texture, button_anim, this, sheet);
	save_button = App->gui->AddButton(17, 120, ui_texture, button_anim, this, sheet);
	load_button = App->gui->AddButton(17, 190, ui_texture, button_anim, this, sheet);
	options_button = App->gui->AddButton(17, 260, ui_texture, button_anim, this, sheet);
	exit_game_button = App->gui->AddButton(17, 330, ui_texture, button_anim, this, sheet);
	resume_label = App->gui->AddLabel(10, 5, "RESUME", BLACK, FREEPIXEL, 50, resume_button);
	save_label = App->gui->AddLabel(10, 5, "SAVE", BLACK, FREEPIXEL, 50, save_button);
	load_label = App->gui->AddLabel(10, 5, "LOAD", BLACK, FREEPIXEL, 50, load_button);
	options_label = App->gui->AddLabel(10, 5, "OPTION", BLACK, FREEPIXEL, 50, options_button);
	exit_game_label = App->gui->AddLabel(10, 5, "EXIT", BLACK, FREEPIXEL, 50, exit_game_button);
}

void Pause::UnloadPause()
{
	App->gui->DeleteUI(sheet);
	App->gui->DeleteUI(resume_button);
	App->gui->DeleteUI(save_button);
	App->gui->DeleteUI(load_button);
	App->gui->DeleteUI(options_button);
	App->gui->DeleteUI(exit_game_button);
	App->gui->DeleteUI(resume_label);
	App->gui->DeleteUI(save_label);
	App->gui->DeleteUI(load_label);
	App->gui->DeleteUI(options_label);
	App->gui->DeleteUI(exit_game_label);
}

void Pause::PauseButtons()
{
	MouseEnter(resume_button);
	MouseEnter(save_button);
	MouseEnter(load_button);
	MouseEnter(options_button);
	MouseEnter(exit_game_button);

	MouseClick();

}

void Pause::MouseEnter(UIElement* element)
{
	if (element->state == FOCUSED)
	{
		element->anim = button_focused_anim;
	}
	if (element->state == MOUSE_LEAVE)
	{
		element->anim = button_anim;
	}
}

void Pause::MouseClick()
{
	if (resume_button->state == L_MOUSE_PRESSED)
	{
		App->paused = false;
		UnloadPause();
	}
	if (save_button->state == L_MOUSE_PRESSED)
	{
		App->SaveGame();
	}
	if (load_button->state == L_MOUSE_PRESSED)
	{
		UnloadPause();
		App->paused = false;
		App->LoadGame();
	}
	if (options_button->state == L_MOUSE_PRESSED)
	{
		UnloadPause();
		LoadOptions();
	}
	if (exit_game_button->state == L_MOUSE_PRESSED)
	{
		App->scene->level = MENU;
		UnloadPause();
		App->paused = false;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}
}

void Pause::LoadOptions()
{
}

void Pause::UnLoadOptions()
{
}

void Pause::OptionsButtons()
{
}
