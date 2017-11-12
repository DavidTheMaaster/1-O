#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Map.h"

j1Collision::j1Collision()
{

	name.create("collision");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_KILL] = false;


	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_KILL] = true;

	matrix[COLLIDER_KILL][COLLIDER_WALL] = false;
	matrix[COLLIDER_KILL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_KILL][COLLIDER_KILL] = false;


}

// Destructor
j1Collision::~j1Collision()
{}


bool j1Collision::Awake() {

	return true;
}


bool j1Collision::Start() {

	return true;

}

bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Collision::Update(float dt)
{
	DebugDraw();
	return true;
}

void j1Collision::DebugDraw()
{
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;
	
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_KILL: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;

		case COLLIDER_SPAWN:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool j1Collision::EraseCollider(Collider* collider)
{
	if (collider != nullptr)
	{
		// we still search for it in case we received a dangling pointer
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}


	return false;
}
// -----------------------------------------------------


bool j1Collision::CheckCollisionRight(SDL_Rect p, iPoint speed)
{
	bool ret = true;

	iPoint vec1;
	iPoint vec2;
	iPoint vec3;

	vec1.x = p.x + p.w + speed.x;
	vec1.y = p.y;

	vec2.x = p.x + p.w + speed.x;
	vec2.y = p.y + p.h / 2;

	vec3.x = (p.x + p.w) + speed.x;
	vec3.y = p.y + p.h;

	vec1 = App->map->WorldToMap(vec1.x, vec1.y);
	vec2 = App->map->WorldToMap(vec2.x, vec2.y);
	vec3 = App->map->WorldToMap(vec3.x, vec3.y);


	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL)
	{
		ret = false;
	}

	if (App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL)
	{
		ret = false;
	}

	if (App->map->logic_layer->data->Get(vec3.x, vec3.y) == WALL)
	{
		ret = false;
	}


	return ret;
}

bool j1Collision::CheckCollisionLeft(SDL_Rect p, iPoint speed)
{
	bool ret = true;

	iPoint vec1;
	iPoint vec2;
	iPoint vec3;

	vec1.x = p.x - speed.x;
	vec1.y = p.y;

	vec2.x = p.x - speed.x;
	vec2.y = p.y + p.h / 2;

	vec3.x = p.x - speed.x;
	vec3.y = p.y + p.h;

	vec1 = App->map->WorldToMap(vec1.x, vec1.y);
	vec2 = App->map->WorldToMap(vec2.x, vec2.y);
	vec3 = App->map->WorldToMap(vec3.x, vec3.y);


	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL)
	{
		ret = false;
	}

	if (App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL)
	{
		ret = false;
	}

	if (App->map->logic_layer->data->Get(vec3.x, vec3.y) == WALL)
	{
		ret = false;
	}


	return ret;
}

bool j1Collision::CheckCollisionUp(SDL_Rect p, iPoint speed)
{
	bool ret = true;

	iPoint vec1;
	iPoint vec2;

	vec1.x = p.x;
	vec1.y = p.y - speed.y;

	vec2.x = p.x + p.w;
	vec2.y = p.y - speed.y;

	vec1 = App->map->WorldToMap(vec1.x, vec1.y);
	vec2 = App->map->WorldToMap(vec2.x, vec2.y);



	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL)
	{
		ret = false;
	}

	if (App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL)
	{
		ret = false;
	}

	return ret;
}

bool j1Collision::CheckCollisionDown(SDL_Rect p, iPoint speed)
{
	bool ret = true;

	iPoint vec1;
	iPoint vec2;

	vec1.x = p.x;
	vec1.y = p.y + p.h + speed.y;

	vec2.x = p.x + p.w;
	vec2.y = p.y + p.h + speed.y;

	vec1 = App->map->WorldToMap(vec1.x, vec1.y);
	vec2 = App->map->WorldToMap(vec2.x, vec2.y);



	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL)
	{
		ret = false;
	
	}

	if (App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL)
	{
		ret = false;
	
	}

	return ret;
}