#include "Menu.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "p2Log.h"
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
	animations = animation_file.child("animations").child("ui").first_child();

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
		;

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

		if (k == 0)
			anim_speed[0] = load_anim->speed;
		k++;
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
	App->tex->UnLoad(ui_texture);
	App->tex->UnLoad(menu_texture);
	App->tex->UnLoad(cross_texture);
	return true;
}


void Menu::LoadMenuUI()
{
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
	hand = App->gui->AddImage(337, 420, ui_texture, hand_anim);
}

void Menu::LoadOptionUI()
{
	App->gui->DeleteUI(hand);
	option_sheet = App->gui->AddImage(490, 35, ui_texture, sheet_anim);
	exit_options = App->gui->AddButton(275, 50, ui_texture, exit_button_anim, this, option_sheet);
	morevolume = App->gui->AddButton(306, 200, ui_texture, right_arrow_anim, this, option_sheet);
	lessvolume = App->gui->AddButton(40, 200, ui_texture, left_arrow_anim, this, option_sheet);
	volume_bar = App->gui->AddImage(85, 200, ui_texture, volume_anim, option_sheet);
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
			App->scene->level = App->scene->MENU_PLAY;			
			LoadPlayUI();
		}
		if (options_ui)
		{
			App->scene->level = App->scene->OPTIONS;
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
		App->gui->DeleteUI(exit_options);
		App->gui->DeleteUI(option_sheet);
		App->gui->DeleteUI(morevolume);
		App->gui->DeleteUI(volume_char);
		App->gui->DeleteUI(lessvolume);
		App->gui->DeleteUI(volume_bar);
		ResetMenu();
	}
	if (morevolume->state == L_MOUSE_PRESSED)
	{
		if (App->audio->volume < 100)
		{
			Mix_VolumeMusic(App->audio->volume++);
			SetVolume();
		}
	}
	if (lessvolume->state == L_MOUSE_PRESSED)
	{
		if (App->audio->volume > 0)
		{
			Mix_VolumeMusic(App->audio->volume--);
			SetVolume();
		}
		if (App->audio->volume == 0)
			Mix_VolumeMusic(0);
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
			ResetMenu();
			App->LoadGame();
		}

		if (new_game_ui)
		{
			App->scene->level = App->scene->level_1;
			App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 1.5);
		}
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
	App->scene->level = App->scene->MENU;
}


void Menu::SetVolume()
{
	std::string s = std::to_string(App->audio->volume);
	volume = (char *)alloca(s.size() + 1);
	memcpy(volume, s.c_str(), s.size() + 1);
	App->gui->DeleteUI(volume_char);
	volume_char = App->gui->AddLabel(158, 191, volume, BLACK, UPHEAVAL, 50, option_sheet);
}

void Menu::UpdateSpeed(float dt)
{
	cross_anim.speed = anim_speed[0] * dt;
}
