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
	virtual ~Player();

	void Update(float dt);
	void GetOffset();

	void Movement();
	void Right();
	void Left();
	void Up();
	void Down();
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
	void UpdateSpeed();
	void Shoot();


public:

	bool jump = false;
	bool jump2 = false;

	int jumps = 2;
	int jump_counter = 2;
	int ammo = 10;

	uint jump_fx = 0;
	uint shoot_fx = 0;

	uint anim_speed[4];

	//Animations

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
