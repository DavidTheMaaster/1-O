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

	void Jump();
	bool CanJump();

	void DoubleJump();
	bool CanDoubleJump();

	void Hover();

	void CameraMovement();


public: 
	int current;

	SDL_Texture* texture = nullptr;

	iPoint speed;
	SDL_Rect p;

	bool flip;
	bool jump;
	bool jump2;

	int jumps;


	int jump_counter;


	//Animations
	Animation* current_animation = nullptr;
	Animation* load_anim = nullptr;
	Animation idle;
	Animation walk;
	Animation hover;

	enum Animations
	{
		IDLE,
		WALK,
		HOVER
	};

private:
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;
	pugi::xml_node rect;
};



#endif