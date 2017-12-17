#ifndef _MENU_H__
#define _MENU_H__

#include "j1Module.h"
#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "Slider.h"
#include "Animation.h"

struct SDL_Texture;

enum UiAnimations
{
	CROSS,
	SHEET,
	BUTTONS,
	EXIT,
	HAND,
	VOLUME,
	LEFT_ARROW,
	RIGHT_ARROW,
	LINE,
	LOGO,
	ZAP,
	TEXT_BOX = 13,
	CREDITS,
};

class Menu : public j1Module
{
public:
	Menu();
	virtual	~Menu();
	bool Awake(pugi::xml_node& node);
	bool Start();
	bool Update(float dt);
	bool CleanUp();


	bool Animations();
	void HandAnimation();

	void ResetMenu();

	void LoadMenuUI();
	void UnLoadMenuUI();

	void LoadOptionUI();
	void UnLoadOptionUI();

	void LoadPlayUI();
	void UnLoadPlayUI();

	void LoadCredits();
	void UnLoadCredits();

	void MenuButtons();
	void OptionButtons();
	void PlayButtons();
	void CreditButton();

	void SetVolume();
	void UpdateSpeed(float dt);



public:
	bool exit_game = true;

public:
	SDL_Texture *menu_texture, *ui_texture, *cross_texture;
	uint button_focused_fx;
	uint cross_click_fx;
	uint back_fx;

	Animation sheet_anim, line;
	Animation button_anim;
	Animation exit_button_anim;
	Animation hand_anim;
	Animation text_box_anim;
	Animation volume_anim;
	Animation left_arrow_anim;
	Animation right_arrow_anim;
	Animation cross_anim;
	Animation logo_anim;
	Animation zap_anim;
	Animation credit_anim;


	Button* play, *options, *exit, *exit_options, *morevolume, *lessvolume, *new_game_button, *continue_button, *zap, *morefps, *lessfps, *credits;
	Image* sheet, *option_sheet, *hand, *cross, *logo, *frame_rate_cap, *textbox;
	Label* volume_char, *continue_label, *new_game_label, *volume_label, *fps_label, *fps_cap_label, *disclaimer, *license, *authors;
	Slider* volume_bar;

	bool play_ui, options_ui, exit_ui, new_game_ui, continue_ui;
	bool move = true;
	bool move2 = false;
	bool move3 = false;
	bool move4 = false;


	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;
	pugi::xml_node rect;



	int i = 0;
	uint current;
	Animation * load_anim;
	uint anim_speed[2];
	bool change;
	char* volume;
};



#endif // !_MENU_H__

