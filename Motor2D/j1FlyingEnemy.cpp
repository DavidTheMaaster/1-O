#include "j1FlyingEnemy.h"
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

j1FlyingEnemy::j1FlyingEnemy()
{
	name.create("flyingenemy");
}

j1FlyingEnemy::~j1FlyingEnemy()
{
}

bool j1FlyingEnemy::Awake(pugi::xml_node& config)
{

	LOG("Loading animations");

	bool ret = true;

	/*
	animation_file.load_file(config.child("file").attribute("folder").as_string());
	animations = animation_file.child("animations").child("player").first_child();

	if (animations == NULL)
	{
		LOG("Could not load animations");
		ret = false;
	}

	while (animations != NULL) {
		attributes = animations.child("attributes");
		rect = animations.first_child();

		current = attributes.attribute("id").as_int();

		if (current == FLY)
		{
			load_anim = &fly;
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

		}

		animations = animations.next_sibling();

	}

	if (animations == NULL)
	{
		animations = animation_file.child("animations").child("player");
	}
	*/

	return ret;
}


bool j1FlyingEnemy::Start()
{
	bool ret = true;
	LOG("Loading flying enemy");
	speed.x = 4; speed.y = 8;
	r.x = 350; r.y = 100;
	r.w = 20; r.h = 20;
	current_animation = &fly;
	flip = false;
	found = false;
	texture = App->tex->Load(animations.child("texture").child("folder").attribute("file").as_string());
	return ret;
}

bool j1FlyingEnemy::CleanUp()
{
	bool ret = true;
	LOG("Unloading player");

	App->tex->UnLoad(texture);
	return ret;
}

bool j1FlyingEnemy::Update(float dt)
{
	bool ret = true;

	current_animation = &fly;

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		path_index = 0;
		enemy_position = App->map->WorldToMap(r.x, r.y);
		player_position = App->map->WorldToMap(App->player->p.x, App->player->p.y);
		App->pathfinding->CreatePath(enemy_position, player_position);
	//	found = true;
	}

	Movement();
	
	Draw();

	//DEBUG


	return ret;
}


void j1FlyingEnemy::Draw()
{
	if ((App->scene->level == 2 || App->scene->level == 3) && App->fadetoblack->IsFading() == false)
	{
		iPoint offset;
		offset = GetOffset(offset.x, offset.y);

		SDL_Rect rect = current_animation->GetCurrentFrame();

		App->render->DrawQuad(r, 255, 0, 0, 255);
		App->render->Blit(texture, r.x - offset.x, r.y - offset.y, &rect, flip);
	}

}

void j1FlyingEnemy::Movement()
{
	
	if ((App->scene->level == 2 || App->scene->level == 3) && App->fadetoblack->IsFading() == false)
	{
		if (found) {
			if (App->pathfinding->last_path[path_index + 1].x == enemy_position.x && App->pathfinding->last_path[path_index].y == enemy_position.y)
			{
				if (path_index + 1 != App->pathfinding->last_path.Count())
				{
					path_index++;
					if (App->pathfinding->last_path[path_index].x - enemy_position.x == -1)
					{
						iPoint movement = App->map->MapToWorld(App->pathfinding->last_path[path_index].x, App->pathfinding->last_path[path_index].y);
						if (r.x > movement.x) {
							r.x -= 1;
						}

					}
					else if (App->pathfinding->last_path[path_index].x - enemy_position.x == 1)
					{
						iPoint movement = App->map->MapToWorld(App->pathfinding->last_path[path_index].x, App->pathfinding->last_path[path_index].y);
						if (r.x < movement.x) {
							r.x += 1;
						}
					}
					else if (App->pathfinding->last_path[path_index].y - enemy_position.y == 1)
					{
						iPoint movement = App->map->MapToWorld(App->pathfinding->last_path[path_index].x, App->pathfinding->last_path[path_index].y);
						if (r.y < movement.y) {
							r.y += 1;
						}
					}
					else if (App->pathfinding->last_path[path_index].y - enemy_position.y == -1)
					{
						iPoint movement = App->map->MapToWorld(App->pathfinding->last_path[path_index].x, App->pathfinding->last_path[path_index].y);
						if (r.y > movement.y) {
							r.y -= 1;
						}
					}
					else
					{
						path_index = 0;
						found = false;
					}



				}
			}
		}
	
	}
	
}

iPoint j1FlyingEnemy::GetOffset(int x, int y)
{
	iPoint offset;

	if (current_animation == &fly) {
		x = animations.child("idle").child("attributes").attribute("offset_x").as_int(0);
		y = animations.child("idle").child("attributes").attribute("offset_y").as_int(0);
	}

	offset.x = x;
	offset.y = y;

	return iPoint(offset);
}