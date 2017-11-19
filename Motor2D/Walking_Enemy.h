#ifndef __WALKING_ENEMY_H__
#define __WALKING_ENEMY_H__

#include "Entity.h"
#include "p2DynArray.h"
#include "p2Point.h"
enum Direction;

class Walking_Enemy : public Entity
{
private:

	Animation walk;
	Animation idle;

public:
	iPoint originalpos;

	Walking_Enemy(int x, int y);
	void Update(float dt);
	void Movement();
	bool CanStartMovement();
	void GetOffset();
	void UpdateSpeed();

	uint current;

	iPoint spawn;

	bool found;
	bool canmove;
	bool agro;

	int radius;
	int big_radius;
	uint anim_speed[2];

	p2DynArray<iPoint> fly_path;
	iPoint player_position;
	iPoint enemy_position;

	int path_index;
	bool omw = false;
	bool dead = false;
	float dt;


	Collider* collider = nullptr;

	//Animations
	Animation* load_anim = nullptr;


};
#endif
