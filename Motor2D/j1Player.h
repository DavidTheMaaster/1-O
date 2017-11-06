#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"


struct SDL_Texture;
struct Collider;

class j1Player : public j1Module
{
public: 

	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node&);

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	void Draw();
	iPoint GetOffset(int x, int y);

	void Right();
	void Left();

	void Gravity();




public: 
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;
	pugi::xml_node rect;
	p2SString current;

	SDL_Texture* texture = nullptr;

	iPoint speed;
	SDL_Rect p;

	bool flip;

	//Animations
	Animation* load_anim = nullptr;
	Animation idle;
	Animation walk;
	Animation* current_animation = nullptr;

};



#endif