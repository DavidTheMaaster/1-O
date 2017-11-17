#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"


struct SDL_Texture;
struct Collider;

class Player : public Entity
{
public:
	
	Player(int x, int y);

	void Update(float dt);
	iPoint GetOffset(int x, int y);

	void Movement();
	void Right();
	void Left();
	void Gravity();
	void Jump();
	bool CanJump();
	void DoubleJump();
	bool CanDoubleJump();
	void Hover();
	void CameraMovement();
	void Dead();
	void Respawn();
	void CheckIfChange();
	void MovePixels(uint state);


public:
	int current;
	iPoint originalpos;

	float dt = 0;

	int pixels = 0;

	iPoint spawn;
	bool lvl2;
	bool hidden_level;

	bool jump;
	bool jump2;
	bool dead;

	int jumps;
	int jump_counter;

	//Animations
	Animation* current_animation = nullptr;
	Animation* load_anim = nullptr;
	Animation idle;
	Animation walk;
	Animation hover;
	Animation die;

	enum Animations
	{
		IDLE,
		WALK,
		HOVER,
		DEAD
	};

	enum State
	{
		right,
		left,
		down,
		up
	};

	enum Logic
	{
		DIE = 4,
	};

};



#endif
