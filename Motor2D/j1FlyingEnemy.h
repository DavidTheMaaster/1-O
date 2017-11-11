#ifndef __j1FLYINGENEMY_H__
#define __j1FLYINGENEMY_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "p2Point.h"


struct SDL_Texture;
struct Collider;

class j1FlyingEnemy : public j1Module
{
public:

	j1FlyingEnemy();
	~j1FlyingEnemy();

	bool Awake(pugi::xml_node&);

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	void Draw();
	iPoint GetOffset(int x, int y);
	bool GetFlip();

	void Movement();
	bool CanStartMovement();


public:

	SDL_Texture* texture = nullptr;

	iPoint speed;
	SDL_Rect r;

	bool flip;
	bool found;
	bool canmove;
	bool back;

	int radius;
	int big_radius;

	p2DynArray<iPoint> fly_path;
	p2DynArray<iPoint> fly_back_path;
	iPoint player_position;
	iPoint enemy_position;
	iPoint start_enemy_position;

	int path_index;
	bool omw = false;
	bool dead = false;

	//Animations
	Animation* load_anim = nullptr;
	Animation fly;

	enum Animations
	{
		FLY,
	};

private:
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;
	pugi::xml_node rect;
};

#endif