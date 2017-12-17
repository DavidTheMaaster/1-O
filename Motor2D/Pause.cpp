#include "j1Audio.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1Input.h"
#include "Pause.h"
#include "Menu.h"
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
	animations = animation_file.child("animations").child("ui").child("menu").first_child();

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
		if (current == EXIT_PAUSE)
			load_anim = &exit_button_anim;
		if (current == VOLUME_PAUSE)
			load_anim = &volume_anim;
		if (current == LEFT_ARROW_PAUSE)
			load_anim = &left_arrow_anim;
		if (current == RIGHT_ARROW_PAUSE)
			load_anim = &right_arrow_anim;
		if (current == ZAP_PAUSE)
			load_anim = &zap_anim;


		int i = rect.attribute("id").as_int();
		int j = attributes.attribute("size").as_int();

		while (i < j)
		{
			if (load_anim != nullptr)
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
			else
				i = j;


		}
		load_anim = nullptr;
		animations = animations.next_sibling();

	}
	return true;
}

bool Pause::Start()
{
	ui_texture = App->tex->Load("maps/UI.png");
	button_focused_fx = App->audio->LoadFx("audio/fx/mouse_over.wav");
	back_fx = App->audio->LoadFx("audio/fx/menu_back.wav");
	return true;
}

bool Pause::Update(float dt)
{
	if (App->scene->level == level_1 || App->scene->level == level_2 || App->scene->level == hidden_level)
	{
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			App->paused = !App->paused;
			if (App->paused)
				LoadPause();
			
			else
			{
				UnloadPause();
				UnLoadOptions();
			}
		}
	}

	if (App->paused)
		PauseButtons();
	
	if (options_opened)
		OptionsButtons();
return true;
}

bool Pause::CleanUp()
{
	App->tex->UnLoad(ui_texture);
	App->audio->UnLoadFx(back_fx);
	App->audio->UnLoadFx(button_focused_fx);
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
	exit_game_label = App->gui->AddLabel(10, 5, "EXIT GAME", BLACK, FREEPIXEL, 50, exit_game_button,500);

	q = resume_label->pos.x;
	w = resume_label->pos.y;
	e = save_button->pos.y;
	r = save_button->pos.y;
	t = load_button->pos.y;
	y = load_button->pos.y;
	u = options_button->pos.y;
	i = options_button->pos.y;



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
	if (element->state == MOUSE_ENTER)
	{
		App->audio->PlayFx(button_focused_fx);
	}
}

void Pause::MouseClick()
{
	if (resume_button->state == L_MOUSE_PRESSED)
	{
		App->audio->PlayFx(back_fx);
		App->paused = false;
		UnloadPause();
	}
	if (save_button->state == L_MOUSE_PRESSED)
	{
		App->audio->PlayFx(back_fx);
		App->SaveGame();
		save_button->state = FOCUSED;
	}
	if (load_button->state == L_MOUSE_PRESSED)
	{
		App->audio->PlayFx(back_fx);
		UnloadPause();
		App->paused = false;
		App->LoadGame();
	}
	if (options_button->state == L_MOUSE_PRESSED)
	{
		App->audio->PlayFx(back_fx);
		UnloadPause();
		LoadOptions();
		options_loaded = false;
		options_button->state = FOCUSED;
		
	}
	if (exit_game_button->state == L_MOUSE_PRESSED)
	{
		App->audio->PlayFx(back_fx);
		App->scene->level = MENU;
		UnloadPause();
		App->paused = false;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
	}
}

void Pause::LoadOptions()
{
	if (options_loaded == false)
	{
		option_sheet = App->gui->AddImage(490, 35, ui_texture, sheet_anim);
		exit_options_button = App->gui->AddButton(275, 50, ui_texture, exit_button_anim, this, option_sheet);
		morevolume = App->gui->AddButton(306, 200, ui_texture, right_arrow_anim, this, option_sheet);
		lessvolume = App->gui->AddButton(40, 200, ui_texture, left_arrow_anim, this, option_sheet);
		morefps = App->gui->AddButton(306, 300, ui_texture, right_arrow_anim, this, option_sheet);
		lessfps = App->gui->AddButton(40, 300, ui_texture, left_arrow_anim, this, option_sheet);
		frame_rate_cap = App->gui->AddButton(85, 300, ui_texture, volume_anim, this, option_sheet);
		volume_bar = App->gui->AddSlider(85, 200, ui_texture, volume_anim, this, option_sheet);
		volume_label = App->gui->AddLabel(50, -30, "VOLUME", BLACK, UPHEAVAL, 30, volume_bar);
		volume_char = App->gui->AddLabel(80, 0, "100", BLACK, UPHEAVAL, 30, volume_bar);
		std::string s = std::to_string(App->frame_rate);
		p2SString frame_rate = s.c_str();
		fps_label = App->gui->AddLabel(80, 0, (char*)frame_rate.GetString(), BLACK, UPHEAVAL, 30, frame_rate_cap);
		zap = App->gui->AddButton(App->audio->volume*2 -11, 2, ui_texture, zap_anim, this, volume_bar);
		fps_cap_label = App->gui->AddLabel(75, -30, "FPS", BLACK, UPHEAVAL, 30, frame_rate_cap);
		zap->slider_value = App->audio->volume;
		SetVolume();
		options_loaded = true;
		options_opened = true;

	}

	
}

void Pause::UnLoadOptions()
{
	App->audio->PlayFx(back_fx);
	App->gui->DeleteUI(exit_options_button);
	App->gui->DeleteUI(option_sheet);
	App->gui->DeleteUI(morevolume);
	App->gui->DeleteUI(volume_char);
	App->gui->DeleteUI(lessvolume);
	App->gui->DeleteUI(volume_bar);
	App->gui->DeleteUI(zap);
	App->gui->DeleteUI(morefps);
	App->gui->DeleteUI(lessfps);
	App->gui->DeleteUI(fps_label);
	App->gui->DeleteUI(frame_rate_cap);
	App->gui->DeleteUI(volume_label);
	App->gui->DeleteUI(fps_cap_label);
}

void Pause::OptionsButtons()
{
	if (exit_options_button->state == L_MOUSE_PRESSED)
	{
		options_opened = false;
		UnLoadOptions();
		LoadPause();
	}
	if (morevolume->state == L_MOUSE_PRESSED)
	{
		if (App->audio->volume < 100)
		{
			zap->pos.x += 2;
			App->audio->PlayFx(back_fx);
			App->pause->SetVolume();
		}
	}
	if (lessvolume->state == L_MOUSE_PRESSED)
	{
		if (App->audio->volume > 0)
		{
			zap->pos.x -= 2;
			App->audio->PlayFx(back_fx);
			App->pause->SetVolume();
		}
		if (App->audio->volume == 0)
			Mix_VolumeMusic(0);
	}

	if (morefps->state == L_MOUSE_PRESSED)
	{
		if (App->frame_rate == 30)
		{
			App->frame_rate = 60;
			fps_label->ChangeLabel("60", BLACK);
		}
	}
	if (lessfps->state == L_MOUSE_PRESSED)
	{
		if (App->frame_rate == 60)
		{
			App->frame_rate = 30;
			fps_label->ChangeLabel("30", BLACK);
		}

	}
}

void Pause::SetVolume()
{
	if (zap != nullptr)
	{
		if (zap->slider_value < 0)
			App->audio->volume = 0;
		else if (zap->slider_value > 100)
			App->audio->volume = 100;
		else
			App->audio->volume = zap->slider_value;

		std::string s = std::to_string(App->audio->volume);
		p2SString volume = s.c_str();
		volume_char->ChangeLabel(volume.GetString(), BLACK);
		Mix_VolumeMusic(App->audio->volume);
		Mix_Volume(-1, 128 * App->audio->volume / 100);
	}
}