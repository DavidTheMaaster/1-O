#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
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
	void ButtonInteractions();

	void LoadLevelUI();

	void Pause();

	void CheckChange();

public:
	int level = 0;
	bool justloaded = false;
	bool changeMap;

	uint level_change_fx;
	

	enum Levels
	{
		MENU,
		OPTIONS,
		MENU_PLAY,
		level_1,
		level_2,
		hidden_level,
		congrats,
	};

	int player_lifes = 5;
	int ammo = 10;
	int urns = 0;
	float time = 0;
	int score = 0;


private:
	iPoint mouse;
	pugi::xml_node level_name;
	SDL_Texture* ui_texture;
	Animation sheet;
};

#endif // __j1SCENE_H__