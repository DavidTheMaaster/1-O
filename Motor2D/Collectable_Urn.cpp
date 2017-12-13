#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1FadetoBlack.h"
#include "Collectable_Urn.h"
#include "Entity.h"
#include "j1Entities.h"

Collectable_Urn::Collectable_Urn(int x, int y) : Entity(x, y)
{
	//Animations here
	LOG("Loading flying enemy animations");

	bool ret = true;

	animation_file.load_file("animations.xml");
	animations = animation_file.child("animations").child("enemies").child("flying_enemy");

	if (animations == NULL)
	{
		LOG("Could not load animations");
		ret = false;
	}

	attributes = animations.child("attributes");
	rect = animations.first_child();

	load_anim = &anim;

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

	flying_enemy_animation = &anim;
	//

	r.x = x; r.y = y;
	r.w = 30; r.h = 30;
	anim_speed = anim.speed;
	id = 3;

	//Collider
	collider = App->collision->AddCollider(r, COLLIDER_COLLECTIBLE, App->entities);
}

void Collectable_Urn::Update(float dt)
{
	this->dt = dt;
	GetOffset();
	UpdateSpeed();
	collider->SetPos(r.x, r.y);
}

void Collectable_Urn::GetOffset()
{
	offset.x = animations.child("flying_enemy").child("attributes").attribute("offset_x").as_int(0);
	offset.y = animations.child("flying_enemy").child("attributes").attribute("offset_y").as_int(0);
}

void Collectable_Urn::UpdateSpeed()
{
	anim.speed = anim_speed * dt;
}