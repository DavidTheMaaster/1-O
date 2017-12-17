#ifndef __COLLECTABLE_URN_H__
#define __COLLECTABLE_URN_H__

#include "Entity.h"

enum Direction;

class Collectable_Urn : public Entity
{
private:

	Animation anim;

public:

	Collectable_Urn(int x, int y);
	void Update(float dt);
	void GetOffset();
	void UpdateSpeed();

	uint anim_speed = 0;

};
#endif