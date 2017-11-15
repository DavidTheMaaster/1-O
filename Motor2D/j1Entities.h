#ifndef __j1Entities_H__
#define __j1Entities_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2Point.h"
#define MAX_ENTITIES 100

enum ENTITY_TYPES
{
	NO_TYPE,
	PLAYER,
	ENEMY_FLY,
	ENEMY_WALK,
};

class Entity;

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
	int x, y;
};

class j1Entities : public j1Module
{
public:

	j1Entities();
	~j1Entities();

	bool Start();
	bool PreUpdate();
	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddEntity(ENTITY_TYPES type, int x, int y);


private:

	void SpawnEntity(const EntityInfo& info);

private:
	EntityInfo queue[MAX_ENTITIES];
	Entity* entities[MAX_ENTITIES];
	SDL_Texture* enemies_textures;

	p2DynArray<iPoint> flyPositions;
	p2DynArray<iPoint> walkPositions;


};
#endif
