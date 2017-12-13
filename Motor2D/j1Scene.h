#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Button.h"
#include "Image.h"
#include "Animation.h"

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
	void SetUI();

	bool Animations();
	void HandAnimation();

	void ResetMenu();

	void CheckChange();

public:
	int level = 0;
	bool justloaded = false;
	bool changeMap;
<<<<<<< HEAD
	SDL_Texture* menu_texture, *buttons, *cross_texture, *hand_texture, *sheet_text, *option_sheet_text, *exit_options_text;
	Button* play, *options, *exit, *exit_options, *morevolume, *lessvolume;
	Image* sheet, *option_sheet, *hand, *cross; 
=======
	uint player_lifes = 5;
	SDL_Texture* menu_texture, *buttons, *cross_texture, *hand_texture;
	Button* play, *options, *exit;
	Image* hand; 
>>>>>>> origin/master


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
		CROSS
	};

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

	uint anim_speed;

	void UpdateSpeed(float dt);
};

#endif // __j1SCENE_H__