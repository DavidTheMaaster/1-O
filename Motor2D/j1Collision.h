#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 1000

#include "j1Module.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_KILL,
	COLLIDER_SPAWN,
	COLLIDER_PLAYERSHOOT,
	COLLIDER_COLLECTIBLE,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collision : public j1Module
{
public:

	j1Collision();
	~j1Collision();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	bool EraseCollider(Collider* collider);
	void DebugDraw();

	bool CheckCollisionRight(SDL_Rect p, iPoint speed);
	bool CheckCollisionLeft(SDL_Rect p, iPoint speed);
	bool CheckCollisionUp(SDL_Rect p, iPoint speed);
	bool CheckCollisionDown(SDL_Rect p, iPoint speed);
	void GetPixels(SDL_Rect p, int state);

	uint pixels = 0;
	
private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;

	SDL_Rect aux;

	enum Logics
	{
		NONE = 0,
		WALL = 19,
		DEAD = 4,
		LVL2 = 9,
		HIDDEN_LEVEL = 24,
		FINISH = 23,
	};

	enum State
	{
		right,
		left,
		down,
		up
	};


};

#endif // __ModuleCollision_H__
