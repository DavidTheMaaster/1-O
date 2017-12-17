#include "Menu.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"




Menu::Menu()
{
	name.create("menu");
}

Menu::~Menu()
{
}

bool Menu::Awake(pugi::xml_node& node)
{

	animation_file.load_file("animations.xml");
	animations = animation_file.child("animations").child("ui").child("menu").first_child();

	if (animations == NULL)
	{
		LOG("Could not load animations");
	}


	int k = 0;

	while (animations != NULL) {
		attributes = animations.child("attributes");
		rect = animations.first_child();

		current = attributes.attribute("id").as_int();

		if (current == CROSS)
			load_anim = &cross_anim;
		if (current == SHEET)
			load_anim = &sheet_anim;
		if (current == BUTTONS)
			load_anim = &button_anim;
		if (current == EXIT)
			load_anim = &exit_button_anim;
		if (current == HAND)
			load_anim = &hand_anim;
		if (current == VOLUME)
			load_anim = &volume_anim;
		if (current == LEFT_ARROW)
			load_anim = &left_arrow_anim;
		if (current == RIGHT_ARROW)
			load_anim = &right_arrow_anim;
		if (current == LINE)
			load_anim = &line;
		if (current == LOGO)
			load_anim = &logo_anim;
		if (current == ZAP)
			load_anim = &zap_anim;
		if (current == TEXT_BOX)
			load_anim = &text_box_anim;
		if (current == CREDITS)
			load_anim = &credit_anim;
	

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

		if (k == 0)
			anim_speed[0] = load_anim->speed;
		k++;
		load_anim = nullptr;
		animations = animations.next_sibling();

	}
	return true;
}

bool Menu::Start()
{

	ui_texture = App->tex->Load("maps/UI.png");
	menu_texture = App->tex->Load("maps/menu.png");
	cross_texture = App->tex->Load("maps/cross.png");
	cross_click_fx = App->audio->LoadFx("audio/fx/pencil_lines.wav");
	button_focused_fx = App->audio->LoadFx("audio/fx/mouse_over.wav");
	back_fx = App->audio->LoadFx("audio/fx/menu_back.wav");

	return true;
}

bool Menu::Update(float dt)
{
	UpdateSpeed(dt);
	return true;
}

bool Menu::CleanUp()
{
	App->audio->UnLoadFx(cross_click_fx);
	App->audio->UnLoadFx(button_focused_fx);
	App->audio->UnLoadFx(back_fx);
	App->tex->UnLoad(ui_texture);
	App->tex->UnLoad(menu_texture);
	App->tex->UnLoad(cross_texture);
	return true;
}


void Menu::LoadMenuUI()
{
	App->audio->StopMusic();
	App->audio->PlayMusic("audio/music/menu_music.ogg");
	App->gui->AddImage(0, 0, menu_texture);
	sheet = App->gui->AddImage(490, 35, ui_texture, sheet_anim);
	play = App->gui->AddButton(47, 333, ui_texture, button_anim, this, sheet);
	options = App->gui->AddButton(160, 333, ui_texture, button_anim, this, sheet);
	exit = App->gui->AddButton(274, 333, ui_texture, button_anim, this, sheet);
	App->gui->AddLabel(47, 275, "PLAY", BLACK, UPHEAVAL, 20, sheet);
	App->gui->AddLabel(40, 305, "JOGAR", BLACK, UPHEAVAL, 20, sheet);
	App->gui->AddLabel(145, 275, "OPTIONS", BLACK, UPHEAVAL, 20, sheet);
	App->gui->AddLabel(140, 305, "D'OPCIONS", BLACK, UPHEAVAL, 20, sheet);
	App->gui->AddLabel(278, 275, "EXIT", BLACK, UPHEAVAL, 20, sheet);
	App->gui->AddLabel(268, 305, "SORTIR", BLACK, UPHEAVAL, 20, sheet);
	App->gui->AddImage(27, 297, ui_texture, line, sheet);
	App->gui->AddImage(143, 297, ui_texture, line, sheet);
	App->gui->AddImage(257, 297, ui_texture, line, sheet);
	logo = App->gui->AddImage(60, 20, ui_texture, logo_anim, sheet);
	credits = App->gui->AddButton(290, 440, ui_texture, credit_anim, this, sheet);
	hand = App->gui->AddImage(337, 420, ui_texture, hand_anim);
	disclaimer = App->gui->AddLabel(30, 175, "DISCLAIMER: This is a work of fiction. Names, characters, places and incidents either are products of the author's imagination or are used fictitiously. Any resemblance to actual events or persons, living or dead, is entirely coincidental.", VERY_BLACK, MINECRAFT, 13, sheet, 325); textbox = App->gui->AddImage(17, 165, ui_texture, text_box_anim, sheet);
	textbox = App->gui->AddImage(17, 165, ui_texture, text_box_anim, sheet);
	App->scene->LoadHighScore();
}

void Menu::LoadOptionUI()
{
	App->gui->DeleteUI(hand);
	option_sheet = App->gui->AddImage(490, 35, ui_texture, sheet_anim);
	exit_options = App->gui->AddButton(275, 50, ui_texture, exit_button_anim, this, option_sheet);
	morevolume = App->gui->AddButton(306, 200, ui_texture, right_arrow_anim, this, option_sheet);
	lessvolume = App->gui->AddButton(40, 200, ui_texture, left_arrow_anim, this, option_sheet);
	morefps = App->gui->AddButton(306, 300, ui_texture, right_arrow_anim, this, option_sheet);
	lessfps = App->gui->AddButton(40, 300, ui_texture, left_arrow_anim, this, option_sheet);
	frame_rate_cap = App->gui->AddImage(85, 300, ui_texture, volume_anim, option_sheet);
	volume_bar = App->gui->AddSlider(85, 200, ui_texture, volume_anim, this, option_sheet);
	volume_label = App->gui->AddLabel(50, -30, "VOLUME", BLACK, UPHEAVAL, 30, volume_bar);
	volume_char = App->gui->AddLabel(80, 0, "100", BLACK, UPHEAVAL, 30, volume_bar);
	std::string s = std::to_string(App->frame_rate);
	p2SString frame_rate = s.c_str();
	fps_label = App->gui->AddLabel(80, 0, (char*)frame_rate.GetString(), BLACK, UPHEAVAL, 30, frame_rate_cap);
	zap = App->gui->AddButton(App->audio->volume * 2 - 11, 2, ui_texture, zap_anim, this, volume_bar);
	fps_cap_label = App->gui->AddLabel(75, -30, "FPS", BLACK, UPHEAVAL, 30, frame_rate_cap);
	zap->slider_value = App->audio->volume;
	SetVolume();
}

void Menu::LoadPlayUI()
{
	App->gui->DeleteUI(hand);
	option_sheet = App->gui->AddImage(490, 35, ui_texture, sheet_anim);
	new_game_label = App->gui->AddLabel(60, 300, "NEW GAME", BLACK, UPHEAVAL, 20, sheet);
	continue_label = App->gui->AddLabel(235, 300, "CONTINUE", BLACK, UPHEAVAL, 20, sheet);
	new_game_button = App->gui->AddButton(70, 333, ui_texture, button_anim, this, sheet);
	continue_button = App->gui->AddButton(251, 333, ui_texture, button_anim, this, sheet);
	exit_options = App->gui->AddButton(275, 50, ui_texture, exit_button_anim, this, sheet);
	hand = App->gui->AddImage(360, 420, ui_texture, hand_anim);

}

void Menu::LoadCredits()
{
	sheet = App->gui->AddImage(490, 35, ui_texture, sheet_anim);
	exit_options = App->gui->AddButton(275, 10, ui_texture, exit_button_anim, this, sheet);
	authors = App->gui->AddLabel(20, 70, "AUTHORS: DAVID LOZANO & JOAN VALIENTE", BLACK, ARIAL, 18, sheet, 300);
	license = App->gui->AddLabel(20, 90, "ACopyright JS Foundation and other contributors, https://js.foundation/This software consists of voluntary contributions made by many individuals.For exact contribution history, see the revision history available at https ://github.com/DavidTheMaaster/1-O The following license applies to all parts of this software except as documented below : Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. All files located in the node_modules and external directories are externally maintained libraries used by this software which have their own licenses; we recommend you read them, as their terms may differ from the terms above.", BLACK, ARIAL, 13, sheet,325);
}

void Menu::UnLoadCredits()
{
	App->gui->DeleteUI(sheet);
	App->gui->DeleteUI(exit_options);
	App->gui->DeleteUI(authors);
	App->gui->DeleteUI(license);

}

void Menu::MenuButtons()
{
	if (change == false)
	{
		if (play->state == MOUSE_ENTER) {
			hand->pos.x = 337;
			App->audio->PlayFx(button_focused_fx);
		}
		if (options->state == MOUSE_ENTER) {
			hand->pos.x = 450;
			App->audio->PlayFx(button_focused_fx);
		}
		if (exit->state == MOUSE_ENTER) {
			hand->pos.x = 563;
			App->audio->PlayFx(button_focused_fx);
		}
		if (credits->state == L_MOUSE_PRESSED)
		{
			LoadCredits();
			App->scene->level = MENU_CREDIT;
			credits->state = FOCUSED;
		}

		if (play->state == L_MOUSE_PRESSED)
		{
			cross = App->gui->AddImage(0, 0, cross_texture, cross_anim, play);
			App->audio->PlayFx(cross_click_fx);
			App->gui->DeleteUI(hand);
			hand = App->gui->AddImage(337, 420, ui_texture, hand_anim);
			play_ui = true;
			change = true;
		}
		if (options->state == L_MOUSE_PRESSED)
		{
			cross = App->gui->AddImage(0, 0, cross_texture, cross_anim, options);
			App->audio->PlayFx(cross_click_fx);
			App->gui->DeleteUI(hand);
			hand = App->gui->AddImage(450, 420, ui_texture, hand_anim);
			options_ui = true;
			change = true;
		}

		if (exit->state == L_MOUSE_PRESSED)
		{
			cross = App->gui->AddImage(0, 0, cross_texture, cross_anim, exit);
			App->audio->PlayFx(cross_click_fx);
			App->gui->DeleteUI(hand);
			hand = App->gui->AddImage(563, 420, ui_texture, hand_anim);
			exit_ui = true;
			change = true;
		}
	}

	if (Animations())
	{
		if (play_ui)
		{
			App->scene->level = MENU_PLAY;			
			LoadPlayUI();
		}
		if (options_ui)
		{
			App->scene->level = OPTIONS;
			LoadOptionUI();
		}
		if (exit_ui)
			exit_game = false;
	}
}

void Menu::OptionButtons()
{
	if (exit_options->state == L_MOUSE_PRESSED)
	{
		App->audio->PlayFx(back_fx);
		App->gui->DeleteUI(exit_options);
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
		ResetMenu();
	}
	if (morevolume->state == L_MOUSE_PRESSED)
	{
		if (App->audio->volume < 100)
		{
			zap->pos.x += 2;
			App->audio->PlayFx(back_fx);
			SetVolume();
		}
	}
	if (lessvolume->state == L_MOUSE_PRESSED)
	{
		if (App->audio->volume > 0)
		{
			zap->pos.x -= 2;
			App->audio->PlayFx(back_fx);
			SetVolume();
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

void Menu::PlayButtons()
{
	if (new_game_button->state == MOUSE_ENTER) {
		hand->pos.x = 357;
		App->audio->PlayFx(button_focused_fx);
		
	}
	if (continue_button->state == MOUSE_ENTER) {
		hand->pos.x = 545;
		App->audio->PlayFx(button_focused_fx);
		
	}

	if (exit_options->state == L_MOUSE_PRESSED)
	
	{
		App->gui->DeleteUI(option_sheet);
		App->gui->DeleteUI(new_game_label);
		App->gui->DeleteUI(continue_label);
		App->gui->DeleteUI(new_game_button);
		App->gui->DeleteUI(continue_button);
		App->gui->DeleteUI(exit_options);
		App->gui->DeleteUI(hand);
		ResetMenu();
	}

	if (new_game_button->state == L_MOUSE_PRESSED)
	{
		cross = App->gui->AddImage(0, 0, cross_texture, cross_anim, new_game_button);
		App->audio->PlayFx(cross_click_fx);
		App->gui->DeleteUI(hand);
		hand = App->gui->AddImage(357, 420, ui_texture, hand_anim);
		new_game_ui = true;
		change = true;
	}
	if (continue_button->state == L_MOUSE_PRESSED)
	{
		cross = App->gui->AddImage(0, 0, cross_texture, cross_anim, continue_button);
		App->audio->PlayFx(cross_click_fx);
		App->gui->DeleteUI(hand);
		hand = App->gui->AddImage(545, 420, ui_texture, hand_anim);
		continue_ui = true;
		change = true;
	}

	if (Animations())
	{
		if (continue_ui)
		{
			App->gui->DeleteUI(option_sheet);
			App->gui->DeleteUI(new_game_label);
			App->gui->DeleteUI(continue_label);
			App->gui->DeleteUI(new_game_button);
			App->gui->DeleteUI(continue_button);
			App->gui->DeleteUI(exit_options);
			App->gui->DeleteUI(cross);
			App->gui->DeleteUI(hand);
			App->LoadGame();
		}

		if (new_game_ui)
		{
			App->scene->level =level_1;
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
		}
	}
}

void Menu::CreditButton()
{
	if (exit_options->state == L_MOUSE_PRESSED)
	{
		UnLoadCredits();
		App->scene->level = MENU;
	}
}

bool Menu::Animations()
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
	else
	{
		i = 0;
		move = true;
	}
	return ret;
}

void Menu::HandAnimation()
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
	if (move2 == true)
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

void Menu::ResetMenu()
{
	i = 0;
	move = true;
	cross_anim.Reset();
	App->gui->DeleteUI(cross);
	play_ui = options_ui = exit_ui = continue_ui = new_game_ui = false;
	hand = App->gui->AddImage(337, 420, ui_texture, hand_anim);
	App->scene->level = MENU;
}


void Menu::SetVolume()
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
		Mix_VolumeMusic(128 * App->audio->volume/100);
		Mix_Volume(-1, 128 * App->audio->volume / 100);
	}
}

void Menu::UpdateSpeed(float dt)
{
	cross_anim.speed = anim_speed[0] * dt;
}
