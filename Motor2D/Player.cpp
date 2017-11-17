#include "Player.h"
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
#include "Entity.h"
#include "j1Entities.h"


Player::Player(int x, int y) : Entity (x, y)
{

	LOG("Loading animations");

	bool ret = true;

	animation_file.load_file("animations.xml");
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

		if (current == IDLE)
		{
			load_anim = &idle;
		}
		else if (current == WALK)
		{
			load_anim = &walk;
		}
		else if (current == HOVER)
		{
			load_anim = &hover;
		}
		else if (current == DEAD)
		{
			load_anim = &die;
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
			anim_speed[i] = load_anim->speed;

		}

		animations = animations.next_sibling();

	}

	if (animations == NULL)
	{
		animations = animation_file.child("animations").child("player");
	}

	//Collider
	collider = App->collision->AddCollider(r, COLLIDER_PLAYER, App->entities);
	
	player_animation = &idle;

	speed.x = 2; speed.y = 2;
	spawn.x = x; spawn.y = y;
	r.x = spawn.x; r.y = spawn.y;
	r.w = 16; r.h = 59;
	flip = false;
	current_animation = &idle;
	jump_counter = 0;
	flip = false;
	lvl2 = false;
	hidden_level = false;
	App->render->camera.x = 0;
	App->render->camera.y = 0;
	jumps = 2;
}


void Player::Update(float dt)
{
	this->dt = dt;
	GetOffset();
	UpdateSpeed();

	speed.x = floor(250 * dt);
	speed.y = floor(450 * dt);


	current_animation = &idle;

	Movement();
	CameraMovement();
	Gravity();
	Dead();
	CheckIfChange();

	if (dead == true)
	{
		Respawn();
	}

	//DEBUG


	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
	{
		r.x = spawn.x;
		r.y = spawn.y;
		App->render->camera.x = 0;
		dead = false;
	}

	player_hitbox->SetPos(r.x, r.y);

	player_pos.x = r.x; player_pos.y = r.y;

}

void Player::Movement()
{
	if (App->fadetoblack->IsFading() == false && dead == false)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			Right();
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			Left();
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && CanJump()) {
			jump = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
			Hover();
		}
		if (jump == true)
		{
			Jump();
		}

		if (jump2 == true)
		{
			DoubleJump();
		}
	}
}

void Player::Right()
{
	if (App->collision->CheckCollisionRight(r, speed))
	{
		r.x += speed.x;
		flip = false;
		current_animation = &walk;
	}
	else
	{
		MovePixels(right);
		App->collision->pixels = 0;
	}
}


void Player::Left()
{
	if (App->collision->CheckCollisionLeft(r, speed))
	{
		r.x -= speed.x;
		flip = true;
		current_animation = &walk;
	}
	else
	{
		MovePixels(left);
		App->collision->pixels = 0;
	}
}



void Player::Gravity()
{
	if (App->collision->CheckCollisionDown(r, speed) && jump == false && jump2 == false && dead == false)
	{
		r.y += speed.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && CanDoubleJump()) {
		jump2 = true;
		jump_counter = 0;
		jump = false;
	}
	if (App->collision->CheckCollisionDown(r, speed) == false)
	{
		jumps = 2;
		MovePixels(down);
		App->collision->pixels = 0;
	}
}

void Player::Jump()
{
	if (App->collision->CheckCollisionUp(r, speed) && jump2 == false) {
		floor(450 * dt);
		jumps = 1;
		if (jump_counter < 12)
		{
			r.y -= speed.y;
			jump_counter++;
		}
		else if (jump_counter < 15)
		{
			jump_counter++;
		}
		else
		{
			jump_counter = 0;
			jump = false;
		}
	}
	else
	{
		jump_counter = 0;
		jump = false;
		jump = false;
	}
}

bool Player::CanJump()
{
	bool ret = true;
	if (jump == true)
	{
		ret = false;
	}

	if (App->collision->CheckCollisionDown(r, speed) && jump == false)
	{
		ret = false;
	}
	if (jumps == 0)
	{
		ret = false;
	}
	return ret;
}

void Player::DoubleJump()
{
	if (App->collision->CheckCollisionUp(r, speed))
	{
		jumps = 0;
		if (jump_counter < 7)
		{
			r.y -= speed.y;
			jump_counter++;
		}
		else if (jump_counter < 10)
		{
			jump_counter++;
		}
		else
		{
			jump_counter = 0;
			jump2 = false;
		}
	}
	else
	{
		jump2 = false;
		jump_counter = 0;
		jump2 = false;
	}

}

bool Player::CanDoubleJump()
{
	bool ret = false;
	if (App->collision->CheckCollisionDown(r, speed))
	{
		ret = true;
	}
	if (jumps == 0)
	{
		ret = false;
	}
	return ret;
}

void Player::Hover()
{
	speed.y = floor(275 * dt);

	if (App->collision->CheckCollisionDown(r, speed))
	{
		current_animation = &hover;
	}
}

void Player::CameraMovement()
{

	if (r.x > App->win->width / 4 && r.x < 1760) {

		App->render->camera.x = 0 - (r.x * 2 - App->win->width / 2);

	}
}

void Player::Dead()
{
	if (App->collision->ActualTile(r) == DIE)
	{
		dead = true;
	}
	if (dead == true)
	{
		current_animation = &die;
	}
}

void Player::Respawn()
{
	if (current_animation->current_frame == 21)
	{
		current_animation->Reset();
		current_animation = &idle;
		r.x = spawn.x;
		r.y = spawn.y;
		dead = false;
		App->render->camera.x = 0;
	}
}

void Player::CheckIfChange() {

	if (lvl2) {
		App->scene->level = App->scene->level_2;
		App->scene->changeMap = true;
	}

	if (hidden_level) {
		App->scene->level = App->scene->hidden_level;
		App->scene->changeMap = true;
	}
}

void Player::MovePixels(uint state)
{
	if (state == right)
	{
		r.x += App->collision->pixels;
	}
	if (state == left)
	{
		r.x -= App->collision->pixels;
	}
	if (state == down)
	{
		r.y += App->collision->pixels;
	}
}


void Player::GetOffset()
{
	if (current_animation == &idle) {
		offset.x = animations.child("idle").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("idle").child("attributes").attribute("offset_y").as_int(0);
	}

	if (current_animation == &walk) {
		offset.x = animations.child("walk").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("walk").child("attributes").attribute("offset_y").as_int(0);
	}

	if (current_animation == &hover) {
		offset.x = animations.child("hover").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("hover").child("attributes").attribute("offset_y").as_int(0);
	}
	if (current_animation == &die) {
		offset.x = animations.child("dead").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("dead").child("attributes").attribute("offset_y").as_int(0);
	}

}

void Player::UpdateSpeed()
{
	idle.speed = anim_speed[0] * dt;
	walk.speed = anim_speed[1] * dt;
	hover.speed = anim_speed[2] * dt;
	die.speed = anim_speed[3] * dt;

}