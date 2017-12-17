#ifndef __FLYING_ENEMY_H__
#define __FLYING_ENEMY_H__

#include "Entity.h"
#include "p2DynArray.h"
#include "p2Point.h"
enum Direction;

class Flying_Enemy : public Entity
{
private:

	Animation fly;

public:

	Flying_Enemy(int x, int y);
	void Update(float dt);
	void Movement();
	bool CanStartMovement();
	void GetOffset();
	void UpdateSpeed();

	uint anim_speed = 0;

	p2DynArray<iPoint> fly_path;



};

#endif
