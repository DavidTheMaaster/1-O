#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Button.h"
#include "Image.h"
#include "Animation.h"
#include "Label.h"


struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Save & Load
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);


	void GetKeys();
	void ButtonInteractions();

	bool Animations();
	void HandAnimation();

	void ResetMenu();

	void CheckChange();

	void LoadMenuUI();
	void LoadOptionUI();
	void LoadLevelUI();

	void Pause();
	void MenuButtons();
	void OptionButtons();

	void SetVolume();


public:
	int level = 0;
	bool justloaded = false;
	bool changeMap;

	uint level_change_fx;
	uint button_focused_fx;
	uint cross_click_fx;

	SDL_Texture *menu_texture, *ui_texture, *cross_texture;
	Button* play, *options, *exit, *exit_options, *morevolume, *lessvolume;
	Image* sheet, *option_sheet, *hand, *cross, *volume_bar; 
	Label* volume_char;

	Animation sheet_anim, button_anim, exit_button_anim, hand_anim, line, text_box_anim, volume_anim, left_arrow_anim, right_arrow_anim;

	enum Levels
	{
		MENU,
		OPTIONS,
		level_1,
		level_2,
		hidden_level,
		congrats,
	};

	enum UiAnimations
	{
		CROSS,
		SHEET,
		BUTTON,
		EXIT,
		HAND,
		VOLUME,
		LEFT_ARROW,
		RIGHT_ARROW,
		LINE
	};

	int player_lifes;

private:
	iPoint mouse;
	pugi::xml_node level_name;
	Animation cross_anim;
	bool change; 
	int i = 0;

	bool play_ui, options_ui, exit_ui;
	bool move = true;
	bool move2 = false;
	bool move3 = false;
	bool move4 = false;
	//UI ELEMENTS
	bool exit_game;
	

private: 
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;

	pugi::xml_node rect;

	uint current;
	Animation * load_anim;

	uint anim_speed[2];

	char* volume;

	void UpdateSpeed(float dt);
};

#endif // __j1SCENE_H__