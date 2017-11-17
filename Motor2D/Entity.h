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
	Animation* flying_enemy_animation = nullptr;
	Animation* walking_enemy_animation = nullptr;
	Animation* player_animation = nullptr;

public:
	fPoint position;

	iPoint player_pos;

	float hitpoints;
	bool death = false;
	int life;
	iPoint speed;
	iPoint offset;

	SDL_Rect r;
	bool flip;

	uint entity;


	Collider* collider = nullptr;

public:
	Entity(int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;

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
