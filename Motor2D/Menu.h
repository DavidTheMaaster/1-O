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
	SDL_Texture *menu_texture = nullptr, *ui_texture = nullptr, *cross_texture = nullptr;
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


	Button* play = nullptr;
	Button* credits = nullptr;
	Button* options = nullptr;
	Button* exit  = nullptr;
	Button* exit_options = nullptr;
	Button* morevolume = nullptr;
	Button* lessvolume = nullptr;
	Button* new_game_button = nullptr;
	Button* continue_button = nullptr;
	Button* zap = nullptr;
	Button* morefps = nullptr;
	Button* lessfps = nullptr;

	Image* sheet = nullptr;
	Image*option_sheet = nullptr;
	Image* hand = nullptr;
	Image* cross = nullptr;
	Image* logo = nullptr;
	Image* frame_rate_cap = nullptr;
	Image* textbox = nullptr;

	Label* volume_char = nullptr;
	Label*continue_label = nullptr;
	Label*new_game_label = nullptr;
	Label*volume_label = nullptr;
	Label* fps_label = nullptr;
	Label* fps_cap_label = nullptr;
	Label* disclaimer = nullptr;
	Label*license = nullptr;
	Label* authors = nullptr;


	Slider* volume_bar = nullptr;

	bool play_ui =false, options_ui = false, exit_ui = false, new_game_ui = false, continue_ui = false;
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
	Animation * load_anim = nullptr;
	uint anim_speed[2];
	bool change = false;
	char* volume = nullptr;
};



#endif // !_MENU_H__

