#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "PugiXml\src\pugixml.hpp"


struct SDL_Texture;
struct Collider;


class Entity
{
protected:
	Animation* load_anim = nullptr;
	Animation* flying_enemy_animation = nullptr;
	Animation* walking_enemy_animation = nullptr;
	Animation* player_animation = nullptr;
	Animation* urn_animation = nullptr;

public:
	fPoint position;
	bool death = false;
	iPoint speed;
	iPoint offset;
	SDL_Rect r;
	bool flip = false;
	uint id = 0;
	uint type = 0;
	iPoint spawn;
	uint current = 0;	
	float dt = 0;
	Collider *collider = nullptr;

	// Enemies
	int path_index = 0;
	bool omw = false;
	bool dead = false;

	bool found = false;
	bool canmove = false;
	bool agro = false;

	int radius = 0;
	int big_radius = 0;

	iPoint player_position;
	iPoint enemy_position;



public:
	Entity(int x, int y);
	virtual ~Entity();

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);

protected:
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;
	pugi::xml_node rect;
};

#endif // __ENEMY_H__
