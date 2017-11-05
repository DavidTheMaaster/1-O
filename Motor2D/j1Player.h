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

	void Right();
	void Left();

	bool CheckCollision();


public: 
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node rect;

	SDL_Texture* test = nullptr;

	iPoint speed;
	SDL_Rect p;

	//Animations
	Animation* load_anim = nullptr;
	Animation idle;
	
};



#endif