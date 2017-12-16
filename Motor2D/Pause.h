#ifndef _PAUSE_H__
#define _PAUSE_H__

#include "j1Module.h"
#include "Animation.h"
#include "Button.h"
#include "Image.h"
#include "Label.h"

struct SDL_Tecture;

enum Animations_Pause
{
	NO_ANIM,
	BUTTON_PAUSE,
	BUTTON_PAUSE_FOCUSED,
	SHEET_PAUSE,
};

class Pause : public j1Module
{
public:

	Pause();
	virtual	~Pause();

	bool Awake(pugi::xml_node& node);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void LoadPause();
	void UnloadPause();
	void PauseButtons();

	void MouseEnter(UIElement* element);
	void MouseClick();

	void LoadOptions();
	void UnLoadOptions();
	void OptionsButtons();

private:
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;
	pugi::xml_node rect;
	uint current;
	Animation * load_anim;
	SDL_Texture* 	ui_texture = nullptr;

	Image* sheet;
	Button* resume_button;
	Button* save_button;
	Button* load_button;
	Button* options_button;
	Button* exit_game_button;


	Label* resume_label;
	Label* save_label;
	Label* load_label;
	Label* options_label;
	Label* exit_game_label;


	Animation button_anim, button_focused_anim, sheet_anim;
};


#endif // !_PAUSE_H__

