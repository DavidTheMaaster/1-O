#ifndef _PAUSE_H__
#define _PAUSE_H__

#include "j1Module.h"
#include "Animation.h"
#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "Slider.h"

struct SDL_Tecture;

enum Animations_Pause
{
	NO_ANIM,
	SHEET_PAUSE = 1,
	EXIT_PAUSE = 3,
	VOLUME_PAUSE = 5,
	LEFT_ARROW_PAUSE = 6,
	RIGHT_ARROW_PAUSE = 7,
	ZAP_PAUSE = 10,
	BUTTON_PAUSE = 11,
	BUTTON_PAUSE_FOCUSED = 12,

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

	void SetVolume();

private:
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;
	pugi::xml_node rect;
	uint current = 0;
	Animation * load_anim = nullptr;
	SDL_Texture* 	ui_texture = nullptr;

	uint button_focused_fx;
	uint back_fx;

	Image* sheet = nullptr;
	Image* option_sheet = nullptr;

	Button* resume_button = nullptr;
	Button* save_button = nullptr;
	Button* load_button = nullptr;
	Button* options_button = nullptr;
	Button* exit_game_button = nullptr;
	Button* exit_options_button = nullptr;
	Button* morevolume = nullptr;
	Button* lessvolume = nullptr;
	Button* morefps = nullptr;
	Button* lessfps = nullptr;
	Button* zap = nullptr;
	Button* frame_rate_cap = nullptr;
		
	Slider* volume_bar = nullptr;

	Label* resume_label;
	Label* save_label;
	Label* load_label;
	Label* options_label;
	Label* exit_game_label;
	Label* volume_label;
	Label* volume_char;
	Label* fps_label;
	Label* fps_cap_label;


	bool options_opened = false;
	bool options_loaded = false; 

	Animation button_anim, button_focused_anim, sheet_anim;

	Animation exit_button_anim;
	Animation volume_anim;
	Animation left_arrow_anim;
	Animation right_arrow_anim;
	Animation zap_anim;


	int	q;
	int	w;
	int	e;
	int	r;
	int	t;
	int	y;
	int	u;
	int	i;

	
};


#endif // !_PAUSE_H__

