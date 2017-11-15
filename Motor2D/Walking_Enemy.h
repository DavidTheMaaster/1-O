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

public:
	iPoint originalpos;

	Walking_Enemy(int x, int y);
	void Update(float dt);
	void Movement();
	bool CanStartMovement();
	iPoint GetOffset(int x, int y);
	bool GetFlip();

	iPoint spawn;

	bool found;
	bool canmove;
	bool agro;

	int radius;
	int big_radius;

	p2DynArray<iPoint> fly_path;
	iPoint player_position;
	iPoint enemy_position;

	int path_index;
	bool omw = false;
	bool dead = false;

	//Animations
	Animation* load_anim = nullptr;


};
#endif
