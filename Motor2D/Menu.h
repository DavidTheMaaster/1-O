#ifndef _MENU_H__
#define _MENU_H__

#include "j1Module.h"
#include "Button.h"
#include "Image.h"
#include "Label.h"
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
	LINE
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
	void LoadOptionUI();

	void MenuButtons();
	void OptionButtons();

	void SetVolume();
	void UpdateSpeed(float dt);



public:
	bool exit_game = true;

private:
	SDL_Texture *menu_texture, *ui_texture, *cross_texture;
	uint button_focused_fx;
	uint cross_click_fx;

	Animation sheet_anim, line;
	Animation button_anim;
	Animation exit_button_anim;
	Animation hand_anim;
	Animation text_box_anim;
	Animation volume_anim;
	Animation left_arrow_anim;
	Animation right_arrow_anim;
	Animation cross_anim;


	Button* play, *options, *exit, *exit_options, *morevolume, *lessvolume;
	Image* sheet, *option_sheet, *hand, *cross, *volume_bar;
	Label* volume_char;

	bool play_ui, options_ui, exit_ui;
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

