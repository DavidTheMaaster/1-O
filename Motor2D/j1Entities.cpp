#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Entities.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "Flying_Enemy.h"
#include "Walking_Enemy.h"

#define SPAWN_MARGIN 140

j1Entities::j1Entities()
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
		entities[i] = nullptr;

	name.create("entities");
}

j1Entities::~j1Entities()
{
}

bool j1Entities::Start()
{

	enemies_textures = App->tex->Load("textures/enemies.png");


	for (int i = 0; i < flyPositions.Count(); i++)
	{
		iPoint p = App->map->MapToWorld(flyPositions[i].x, flyPositions[i].y);
	}
	for (int i = 0; i < walkPositions.Count(); i++)
	{
		iPoint p = App->map->MapToWorld(walkPositions[i].x, walkPositions[i].y);
		//AddEnemy(ENEMY_WALK, p.x, p.y);
	}
	flyPositions.Clear();
	walkPositions.Clear();
	return true;
}

bool j1Entities::Awake(pugi::xml_node& config)
{

	for (pugi::xml_node it = config.child("fly"); it; it = it.next_sibling())
	{
		flyPositions.PushBack({ it.attribute("posx").as_int(),it.attribute("posy").as_int() });
	}
	for (pugi::xml_node it = config.child("walk"); it; it = it.next_sibling())
	{
		walkPositions.PushBack({ it.attribute("posx").as_int(),it.attribute("posy").as_int() });
	}
	return true;

}
bool j1Entities::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPES::NO_TYPE)
		{
			SpawnEntity(queue[i]);
			queue[i].type = ENTITY_TYPES::NO_TYPE;
		}
	}


	return true;
}

// Called before render is available
bool j1Entities::Update(float dt)
{
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			entities[i]->Update(dt);
		}
	}
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			entities[i]->Draw(enemies_textures);
		}
	}

	return true;
}

bool j1Entities::PostUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			/*if (enemies[i]->position.y < App->render->camera.y)
			{

			delete enemies[i];
			enemies[i] = nullptr;
			}*/
		}
	}

	return true;
}

// Called before quitting
bool j1Entities::CleanUp()
{
	App->tex->UnLoad(enemies_textures);


	// No unload for fxs
	/*
	App->audio->UnloadFX(audio_explosion5);
	*/
	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}

	return true;
}

bool j1Entities::AddEntity(ENTITY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		if (queue[i].type == ENTITY_TYPES::NO_TYPE)
		{
			App->map->data.tile_height;
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Entities::SpawnEntity(const EntityInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; entities[i] != nullptr && i < MAX_ENTITIES; ++i);

	if (i != MAX_ENTITIES)
	{
		switch (info.type)
		{

		case ENTITY_TYPES::NO_TYPE:
			break;
		case ENTITY_TYPES::ENEMY_FLY:
			entities[i] = new Flying_Enemy(info.x, info.y);
			break;
		case ENTITY_TYPES::ENEMY_WALK:
			entities[i] = new Walking_Enemy(info.x, info.y);
			break;
		}

	}
}

void j1Entities::OnCollision(Collider* c1, Collider* c2)
{

}