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


	Walking_Enemy(int x, int y);
	void Update(float dt);
	void Movement();
	bool CanStartMovement();
	void GetOffset();
	void UpdateSpeed();

	uint anim_speed[2];

	p2DynArray<iPoint> fly_path;


};
#endif
