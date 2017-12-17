#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1FadetoBlack.h"
#include "Walking_Enemy.h"
#include "Entity.h"
#include "j1Entities.h"


Walking_Enemy::Walking_Enemy(int x, int y) : Entity(x, y)
{
	//Animations here
	LOG("Loading animations");

	bool ret = true;

	animation_file.load_file("animations.xml");
	animations = animation_file.child("animations").child("enemies").child("walking_enemy_idle");

	if (animations == NULL)
	{
		LOG("Could not load animations");
		ret = false;
	}


	while (animations != NULL) {
		attributes = animations.child("attributes");
		rect = animations.first_child();

		current = attributes.attribute("id").as_int();

		if (current == 1)
		{
			load_anim = &idle;
		}
		else if (current == 2)
		{
			load_anim = &walk;
		}

		int i = rect.attribute("id").as_int();
		int j = attributes.attribute("size").as_int();

		while (i < j)
		{
			SDL_Rect r;
			r.x = rect.attribute("x").as_int();
			r.y = rect.attribute("y").as_int();
			r.w = rect.attribute("w").as_int();
			r.h = rect.attribute("h").as_int();

			load_anim->PushBack({ r.x,r.y,r.w,r.h });

			rect = rect.next_sibling();
			i = rect.attribute("id").as_int();
			load_anim->loop = attributes.attribute("loop").as_bool();
			load_anim->speed = attributes.attribute("speed").as_float();
			radius = attributes.attribute("radius").as_int();
			big_radius = attributes.attribute("big_radius").as_int();

		}
		animations = animations.next_sibling();
	}
	animations = animation_file.child("animations").child("enemies");

	walking_enemy_animation = &idle;
	//
	speed.x = 2; speed.y = 2;
	r.x = x; r.y = y;
	r.w = 20; r.h = 40;
	flip = false;
	found = false;
	agro = false;
	anim_speed[0] = idle.speed;
	anim_speed[1] = walk.speed;
	id = 3;


	//Collider
	collider = App->collision->AddCollider(r, COLLIDER_ENEMY, App->entities);
}

void Walking_Enemy::Update(float dt)
{
	this->dt = dt;
	canmove = CanStartMovement();
	walking_enemy_animation = &idle;
	GetOffset();
	UpdateSpeed();

	speed.x = floor(125 * dt);
	speed.y = floor(250 * dt);

	if (canmove)
	{
		enemy_position = App->map->WorldToMap(r.x, r.y);
		player_position = App->map->WorldToMap(App->entities->player_pos.x, App->entities->player_pos.y);
		path_index = 0;
		found = true;
		canmove = false;
	}


	if (!dead) {
		Movement();
		GetOffset();
	}

	collider->SetPos(r.x, r.y);
}


void Walking_Enemy::Movement()
{
	if (App->fadetoblack->IsFading() == false)
	{
		if (found == true && dead == false) {
			enemy_position = App->map->WorldToMap(r.x, r.y);
			player_position = App->map->WorldToMap(App->entities->player_pos.x, r.y);
			App->pathfinding->CreatePath(enemy_position, player_position, fly_path);

			if (path_index < fly_path.Count())
			{
				iPoint nextTile = App->map->MapToWorld(fly_path[path_index].x, fly_path[path_index].y);
				walking_enemy_animation = &walk;
				if (enemy_position.x <= fly_path[path_index].x && r.x < nextTile.x)
				{
					if (App->collision->CheckCollisionRight(r, speed))
					{
						r.x += speed.x;
					}
					omw = true;
					flip = false;
				}
				else if (enemy_position.x >= fly_path[path_index].x && r.x > nextTile.x)
				{
					if (App->collision->CheckCollisionLeft(r, speed))
					{
						r.x -= speed.x;
					}
					omw = true;
					flip = true;
				}
				else
				{
					omw = false;
				}

				if (!omw) {
					path_index += 1;
				}
			}
		}
		if (App->collision->CheckCollisionDown(r, speed) && dead == false)
			r.y += speed.y;
		
	}
	
}

bool Walking_Enemy::CanStartMovement()
{
	bool ret = false;
	enemy_position = App->map->WorldToMap(r.x, r.y);
	player_position = App->map->WorldToMap(App->entities->player_pos.x, App->entities->player_pos.y);

	int i = player_position.x - enemy_position.x;


	if (player_position.x - enemy_position.x >= -radius && player_position.x - enemy_position.x <= radius)
	{
		if (!found)
		{
			ret = true;
		}
	}
	if ((player_position.x - enemy_position.x <= -big_radius || player_position.x - enemy_position.x >= big_radius) && found == true)
	{
		found = false;
		path_index = 0;
		ret = false;
	}
	return ret;
}

void Walking_Enemy::GetOffset()
{
	if (walking_enemy_animation == &idle)
	{
		offset.x = animations.child("walking_enemy_idle").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("walking_enemy_idle").child("attributes").attribute("offset_y").as_int(0);
	}
	if (walking_enemy_animation == &walk)
	{
		offset.x = animations.child("walking_enemy").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("walking_enemy").child("attributes").attribute("offset_y").as_int(0);
	}
}

void Walking_Enemy::UpdateSpeed() 
{
	idle.speed = anim_speed[0] * dt;
	walk.speed = anim_speed[1] * dt;
}