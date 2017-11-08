#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"


struct SDL_Texture;

class j1Menu : public j1Module
{
public:

	j1Menu();
	~j1Menu();

	bool Awake(pugi::xml_node&);

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	iPoint MousePosition();
	bool ActionMenu();
	void ActionOptions();
	
	void Resolution();
	void Draw();

	void SetResolution(uint x, uint y);


private:
	iPoint mouse;
	iPoint window_size;
	int resolution;
	SDL_Texture* resolution_textures;

	enum Levels
	{
		menu,
		options,
		level_1,
		level_2,
		gameover

	};

	enum ID
	{
		PLAY = 5,
		OPTIONS = 6,
		EXIT = 7
	};

	enum OPTIONS
	{
		minus = 38,
		plus = 39,
		left = 48,
		right = 49,
		exit = 50
	};

private: 
	pugi::xml_document resolutions_file;
	pugi::xml_node resolution_node;

	Animation res_0, res_1, res_2, res_3, res_4;
	Animation* load_animation = nullptr;
	Animation* current_animation = nullptr;
};



#endif
