#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Entities.h"
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
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}
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
		case COLLIDER_ENEMY: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
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


	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL || App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL || App->map->logic_layer->data->Get(vec3.x, vec3.y) == WALL)
	{
		ret = false;
		GetPixels(p, right);
	}

	// Check dispawn and change to level 2
	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == LVL2)
	{
		App->entities->lvl2 = true;
		ret = false;
	}
	if (App->map->logic_layer->data->Get(vec2.x, vec2.y) == LVL2)
	{
		App->entities->lvl2 = true;
		ret = false;
	}
	if (App->map->logic_layer->data->Get(vec3.x, vec3.y) == LVL2)
	{
		App->entities->lvl2 = true;
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


	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL || App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL || App->map->logic_layer->data->Get(vec3.x, vec3.y) == WALL)
	{
		ret = false;
		GetPixels(p, left);
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
	

	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL || App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL)
	{
		ret = false;	
		GetPixels(p,down);
	}



	// Check dispawn and change to hidden level
	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == HIDDEN_LEVEL)
	{
		App->entities->hidden_level = true;
		ret = false;
	}
	if (App->map->logic_layer->data->Get(vec2.x, vec2.y) == HIDDEN_LEVEL)
	{
		App->entities->hidden_level = true;
		ret = false;
	}



	return ret;
}

int j1Collision::ActualTile(SDL_Rect p)
{
	int ret = 0;
	iPoint vec1, vec2, vec3, vec4;

	vec1.x = p.x;
	vec1.y = p.y +p.h;

	vec2.x = p.x + p.w;
	vec2.y = p.y + p.h;

	vec3.x = p.x;
	vec3.y = p.y;
	
	vec4.x = p.x + p.w;
	vec4.y = p.y;

	vec1 = App->map->WorldToMap(vec1.x, vec1.y);
	vec2 = App->map->WorldToMap(vec2.x, vec2.y);
	vec3 = App->map->WorldToMap(vec3.x, vec3.y);
	vec4 = App->map->WorldToMap(vec4.x, vec4.y);

	if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == DEAD || App->map->logic_layer->data->Get(vec2.x, vec2.y) == DEAD || App->map->logic_layer->data->Get(vec3.x, vec3.y) == DEAD || App->map->logic_layer->data->Get(vec4.x, vec4.y) == DEAD)
	{
		ret = DEAD;

	}

	return ret;
}

void j1Collision::GetPixels(SDL_Rect p, int state)
{
	iPoint vec1;
	iPoint vec2;
	iPoint vec3;

	int i = 0;
	int j = 1;

	if (state == right) {
		while (j == 1)
		{
			vec1.x = p.x + p.w + i;
			vec1.y = p.y;

			vec2.x = p.x + p.w + i;
			vec2.y = p.y + p.h / 2;

			vec3.x = p.x + p.w + i;
			vec3.y = p.y + p.h;

			vec1 = App->map->WorldToMap(vec1.x, vec1.y);
			vec2 = App->map->WorldToMap(vec2.x, vec2.y);
			vec3 = App->map->WorldToMap(vec3.x, vec3.y);
			
			if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL || App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL || App->map->logic_layer->data->Get(vec3.x, vec3.y) == WALL)
			{
				j = 0;
				pixels = i - 1;
			}
			i++;
		}
	}
	if (state == left) {
		while (j == 1)
		{
			vec1.x = p.x - i;
			vec1.y = p.y;

			vec2.x = p.x - i;
			vec2.y = p.y + p.h / 2;

			vec3.x = p.x - i;
			vec3.y = p.y + p.h;

			vec1 = App->map->WorldToMap(vec1.x, vec1.y);
			vec2 = App->map->WorldToMap(vec2.x, vec2.y);
			vec3 = App->map->WorldToMap(vec3.x, vec3.y);

			if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL|| App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL|| App->map->logic_layer->data->Get(vec3.x, vec3.y) == WALL)
			{
				j = 0;
				pixels = i - 1;
			}
			i++;
		}
	}
	if (state == down)
	{
		while (j == 1)
		{
			vec1.x = p.x;
			vec1.y = p.y + p.h + i;

			vec2.x = p.x + p.w;
			vec2.y = p.y + p.h + i;

			vec1 = App->map->WorldToMap(vec1.x, vec1.y);
			vec2 = App->map->WorldToMap(vec2.x, vec2.y);


			if (App->map->logic_layer->data->Get(vec1.x, vec1.y) == WALL || App->map->logic_layer->data->Get(vec2.x, vec2.y) == WALL)
			{
				j = 0;
				pixels = i - 1;
			}
			i++;
		}
	}
	
}


bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}