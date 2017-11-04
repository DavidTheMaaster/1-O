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

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	void Draw();

	void Right();
	void Left();

	bool CheckCollision();


public: 
	iPoint speed;
	SDL_Rect r;
};



#endif