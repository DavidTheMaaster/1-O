#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Player.h"
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
	animations = animation_file.child("animations").child("enemies").child("walking_enemy");

	if (animations == NULL)
	{
		LOG("Could not load animations");
		ret = false;
	}

	attributes = animations.child("attributes");
	rect = animations.first_child();

	load_anim = &walk;

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
	animations = animation_file.child("animations").child("enemies");

	//Collider
	collider = App->collision->AddCollider({ (int)position.x, (int)position.y,20,40 }, COLLIDER_ENEMY, App->entities);

	walking_enemy_animation = &walk;
	//
	speed.x = 2; speed.y = 2;
	r.x = x; r.y = y;
	r.w = 20; r.h = 20;
	flip = false;
	found = false;
	agro = false;

}

void Walking_Enemy::Update(float dt)
{
	canmove = CanStartMovement();

	if (canmove)
	{
		enemy_position = App->map->WorldToMap(r.x, r.y);
		player_position = App->map->WorldToMap(App->player->p.x, App->player->p.y);
		path_index = 0;
		found = true;
		canmove = false;
	}


	if (!dead) {
		Movement();
	}
}


void Walking_Enemy::Movement()
{

	if ((App->scene->level == 0 || App->scene->level == 1) && App->fadetoblack->IsFading() == false)
	{
		if (found == true && dead == false) {
			enemy_position = App->map->WorldToMap(r.x, r.y);
			player_position = App->map->WorldToMap(App->player->p.x, r.y);
			App->pathfinding->CreatePath(enemy_position, player_position, fly_path);

			if (path_index < fly_path.Count())
			{
				iPoint nextTile = App->map->MapToWorld(fly_path[path_index].x, fly_path[path_index].y);

				if (enemy_position.x <= fly_path[path_index].x && r.x < nextTile.x)
				{
					r.x += speed.x;
					omw = true;
					flip = false;
				}
				else if (enemy_position.x >= fly_path[path_index].x && r.x > nextTile.x)
				{
					r.x -= speed.x;
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

				if (fly_path[path_index].x == player_position.x && fly_path[path_index].y == player_position.y) {
					path_index = 0;
					dead = true;
				}
			}
		}
	}
	if (App->collision->CheckCollisionDown(r, speed))
	{
		r.y += speed.y;
	}
}

bool Walking_Enemy::CanStartMovement()
{
	bool ret = false;
	enemy_position = App->map->WorldToMap(r.x, r.y);
	player_position = App->map->WorldToMap(App->player->p.x, App->player->p.y);

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

iPoint Walking_Enemy::GetOffset(int x, int y)
{
	iPoint offset;


	x = animations.child("flying_enemy").child("attributes").attribute("offset_x").as_int(0);
	y = animations.child("flying_enemy").child("attributes").attribute("offset_y").as_int(0);

	offset.x = x;
	offset.y = y;

	return iPoint(offset);
}
