#include "Player.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1FadetoBlack.h"
#include "Entity.h"
#include "j1Entities.h"
#include "j1Particles.h"


Player::Player(int x, int y) : Entity (x, y)
{

	LOG("Loading animations");


	animation_file.load_file("animations.xml");
	animations = animation_file.child("animations").child("player").first_child();

	if (animations == NULL)
	{
		LOG("Could not load animations");
	}

	int k = 0;

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

		}

		anim_speed[k] = load_anim->speed;
		k++;

		animations = animations.next_sibling();

	}

	if (animations == NULL)
	{
		animations = animation_file.child("animations").child("player");
	}
	
	player_animation = &idle;
	type = PLAYER;
	speed.x = 2; speed.y = 2;

	
	spawn.x = x; spawn.y = y;

	r.x = spawn.x; r.y = spawn.y;
	r.w = 16; r.h = 59;

	flip = false;
	jump_counter = 0;
	flip = false;

	App->render->camera.x = 0;
	App->render->camera.y = 0;
	jumps = 2;
	id = 1;

	jump_fx = App->audio->LoadFx("audio/fx/jump.wav");
	shoot_fx = App->audio->LoadFx("audio/fx/paper.wav");

	//Collider
	collider = App->collision->AddCollider({ r.x, r.y, r.w, r.h - 7 }, COLLIDER_PLAYER, App->entities);
}

Player::~Player()
{
	App->audio->UnLoadFx(jump);
	App->audio->UnLoadFx(shoot_fx);
}

void Player::Update(float dt)
{
	if (!App->fadetoblack->IsFading())
	{
	this->dt = dt;
	GetOffset();
	UpdateSpeed();

	speed.x = floor(250 * dt);
	speed.y = floor(450 * dt);


	player_animation = &idle;

	Movement();
	CameraMovement();
	Gravity();
	Dead();
	CheckIfChange();

	if (death == true && App->scene->player_lifes != 0)
	{
		Respawn();
	}
	if (App->entities->justloaded) {
		r.x = App->entities->loaded_player_pos.x;
		r.y = App->entities->loaded_player_pos.y;
		App->entities->justloaded = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) {
		Shoot();
		App->audio->PlayFx(shoot_fx);
	}

	collider->SetPos(r.x, r.y);

	App->entities->player_pos.x = r.x; App->entities->player_pos.y = r.y;
	}
}

void Player::Movement()
{
	if (death == false && App->pause == false)
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
		player_animation = &walk;
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
		player_animation = &walk;
	}
	else
	{
		MovePixels(left);
		App->collision->pixels = 0;
	}
}



void Player::Gravity()
{
	if (App->collision->CheckCollisionDown(r, speed) && jump == false && jump2 == false && death == false)
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
		if(jump_counter == 0)
			App->audio->PlayFx(jump_fx);

		if (jump_counter < 0.23/dt)
		{
			r.y -= speed.y;
			jump_counter++;
		}
		
		else if (jump_counter < 0.28/dt)
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
		if (jump_counter == 0)
			App->audio->PlayFx(jump_fx);
		if (jump_counter < 0.15/dt)
		{
			r.y -= speed.y;
			jump_counter++;
		}
		else if (jump_counter < 0.20 / dt)
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
		player_animation = &hover;
	}
}

void Player::CameraMovement()
{

	if (r.x > App->win->width / 4 && r.x < (App->map->data.width - 15) * App->map->data.tile_width) {

		App->render->camera.x = 0 - (r.x * 2 - App->win->width / 2);
	}
	
}

void Player::Dead()
{
	if (death == true)
	{
		player_animation = &die;
		App->scene->player_lifes -= 1;
	}
}

void Player::Respawn()
{
	if (player_animation->current_frame == 21)
	{
		player_animation->Reset();
		player_animation = &idle;
		App->fadetoblack->FadeToBlack(App->scene, App->scene, 0.5);
		death = false;
		App->render->camera.x = 0;
	}
}

void Player::Shoot() {
	if (flip) {
		App->particles->AddParticle(App->particles->LeftShoot, r.x, r.y+10, -12.5, 0, COLLIDER_PLAYERSHOOT);
	}
	else {
		App->particles->AddParticle(App->particles->RightShoot, r.x, r.y+10, 12.5, 0, COLLIDER_PLAYERSHOOT);
	}
}

void Player::CheckIfChange() {

	if (App->entities->lvl2) {
		App->scene->level++;
		App->scene->changeMap = true;
	}

	if (App->entities->hidden_level) {
		App->scene->level = App->scene->hidden_level;
		App->scene->changeMap = true;
	}

	if (App->entities->congrats) {
		App->scene->level = App->scene->congrats;
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
	if (player_animation == &idle) {
		offset.x = animations.child("idle").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("idle").child("attributes").attribute("offset_y").as_int(0);
	}

	if (player_animation == &walk) {
		offset.x = animations.child("walk").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("walk").child("attributes").attribute("offset_y").as_int(0);
	}

	if (player_animation == &hover) {
		offset.x = animations.child("hover").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("hover").child("attributes").attribute("offset_y").as_int(0);
	}
	if (player_animation == &die) {
		offset.x = animations.child("dead").child("attributes").attribute("offset_x").as_int(0);
		offset.y = animations.child("dead").child("attributes").attribute("offset_y").as_int(0);
	}

}

void Player::UpdateSpeed()
{
	idle.speed = anim_speed[0] * dt;
	walk.speed = anim_speed[1] * dt;
	hover.speed = anim_speed[2] * dt;
	die.speed = anim_speed[3] *dt;

}