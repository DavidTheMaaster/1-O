#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include <time.h>
#include <stdlib.h>


#include<stdio.h>

j1Player::j1Player()
{
	name.create("player");

	idleright.PushBack({ 0,0,40,64 });
	idleright.PushBack({ 40,0,40,64 });
	idleright.PushBack({ 80,0,40,64 });
	idleright.PushBack({ 120,0,40,64 });
	idleright.PushBack({ 160,0,40,64 });
	idleright.PushBack({ 200,0,40,64 });
	idleright.PushBack({ 0,64,40,64 });
	idleright.PushBack({ 40,64,40,64 });
	idleright.PushBack({ 80,64,40,64 });
	idleright.PushBack({ 120,64,40,64 });
	idleright.PushBack({ 160,64,40,64 });
	idleright.PushBack({ 200,64,40,64 });
	idleright.PushBack({ 0,129,40,64 });
	idleright.PushBack({ 40,129,40,64 });
	idleright.PushBack({ 80,129,40,64 });
	idleright.speed = 0.5f;

	idleleft.PushBack({ 240,0,40,64 });
	idleleft.PushBack({ 280,0,40,64 });
	idleleft.PushBack({ 320,0,40,64 });
	idleleft.PushBack({ 360,0,40,64 });
	idleleft.PushBack({ 400,0,40,64 });
	idleleft.PushBack({ 440,0,40,64 });
	idleleft.PushBack({ 240,64,40,64 });
	idleleft.PushBack({ 280,64,40,64 });
	idleleft.PushBack({ 320,64,40,64 });
	idleleft.PushBack({ 360,64,40,64 });
	idleleft.PushBack({ 400,64,40,64 });
	idleleft.PushBack({ 440,64,40,64 });
	idleleft.PushBack({ 360,129,40,64 });
	idleleft.PushBack({ 400,129,40,64 });
	idleleft.PushBack({ 440,129,40,64 });
	idleleft.speed = 0.5f;

	walkright.PushBack({ 480,0,60,68 });
	walkright.PushBack({ 540,0,60,68 });
	walkright.PushBack({ 600,0,60,68 });
	walkright.PushBack({ 660,0,60,68 });
	walkright.PushBack({ 720,0,60,68 });
	walkright.PushBack({ 780,0,60,68 });
	walkright.PushBack({ 840,0,60,68 });
	walkright.PushBack({ 900,0,60,68 });
	walkright.PushBack({ 480,68,60,68 });
	walkright.PushBack({ 540,68,60,68 });
	walkright.PushBack({ 600,68,60,68 });
	walkright.PushBack({ 660,68,60,68 });
	walkright.PushBack({ 720,68,60,68 });
	walkright.PushBack({ 780,68,60,68 });
	walkright.PushBack({ 840,68,60,68 });
	walkright.PushBack({ 900,68,60,68 });
	walkright.PushBack({ 480,136,60,68 });
	walkright.PushBack({ 540,136,60,68 });
	walkright.PushBack({ 600,136,60,68 });
	walkright.PushBack({ 660,136,60,68 });
	walkright.PushBack({ 720,136,60,68 });
	walkright.PushBack({ 780,136,60,68 });
	walkright.PushBack({ 840,136,60,68 });
	walkright.PushBack({ 900,136,60,68 });
	walkright.PushBack({ 480,204,60,68 });
	walkright.PushBack({ 540,204,60,68 });
	walkright.PushBack({ 600,204,60,68 });
	walkright.PushBack({ 660,204,60,68 });
	walkright.PushBack({ 720,204,60,68 });
	walkright.PushBack({ 780,204,60,68 });
	walkright.speed = 0.6f;

	walkleft.PushBack({ 1380,0,60,68 });
	walkleft.PushBack({ 1320,0,60,68 });
	walkleft.PushBack({ 1260,0,60,68 });
	walkleft.PushBack({ 1200,0,60,68 });
	walkleft.PushBack({ 1140,0,60,68 });
	walkleft.PushBack({ 1080,0,60,68 });
	walkleft.PushBack({ 1020,0,60,68 });
	walkleft.PushBack({ 960,0,60,68 });
	walkleft.PushBack({ 1380,68,60,68 });
	walkleft.PushBack({ 1320,68,60,68 });
	walkleft.PushBack({ 1260,68,60,68 });
	walkleft.PushBack({ 1200,68,60,68 });
	walkleft.PushBack({ 1140,68,60,68 });
	walkleft.PushBack({ 1080,68,60,68 });
	walkleft.PushBack({ 1020,68,60,68 });
	walkleft.PushBack({ 960,68,60,68 });
	walkleft.PushBack({ 1380,136,60,68 });
	walkleft.PushBack({ 1320,136,60,68 });
	walkleft.PushBack({ 1260,136,60,68 });
	walkleft.PushBack({ 1200,136,60,68 });
	walkleft.PushBack({ 1140,136,60,68 });
	walkleft.PushBack({ 1080,136,60,68 });
	walkleft.PushBack({ 1020,136,60,68 });
	walkleft.PushBack({ 960,136,60,68 });
	walkleft.PushBack({ 1380,204,60,68 });
	walkleft.PushBack({ 1320,204,60,68 });
	walkleft.PushBack({ 1260,204,60,68 });
	walkleft.PushBack({ 1200,204,60,68 });
	walkleft.PushBack({ 1140,204,60,68 });
	walkleft.PushBack({ 1080,204,60,68 });
	walkleft.speed = 0.6f;

	planeright.PushBack({ 0,194,58,80 });
	planeright.PushBack({ 58,194,58,80 });
	planeright.PushBack({ 0,274,58,80 });
	planeright.PushBack({ 58,274,58,80 });
	planeright.speed = 0.1f;

	planeleft.PushBack({ 174,194,58,80 });
	planeleft.PushBack({ 116,194,58,80 });
	planeleft.PushBack({ 174,274,58,80 });
	planeleft.PushBack({ 116,274,58,80 });
	planeleft.speed = 0.1f;

	deadanim.PushBack({ 918, 272, 70,70 });
	deadanim.PushBack({ 988, 272, 70,70 });
	deadanim.PushBack({ 1058, 272, 70,70 });
	deadanim.PushBack({ 1128, 272, 70,70 });
	deadanim.PushBack({ 1198, 272, 70,70 });
	deadanim.PushBack({ 1268, 272, 70,70 });
	deadanim.PushBack({ 1338, 272, 70,70 });
	deadanim.PushBack({ 918, 342, 70,70 });
	deadanim.PushBack({ 988, 342, 70,70 });
	deadanim.PushBack({ 1058, 342, 70,70 });
	deadanim.PushBack({ 1128, 342, 70,70 });
	deadanim.PushBack({ 1198, 342, 70,70 });
	deadanim.PushBack({ 1268, 342, 70,70 });
	deadanim.PushBack({ 1338, 342, 70,70 });
	deadanim.PushBack({ 918, 412, 70,70 });
	deadanim.PushBack({ 988, 412, 70,70 });
	deadanim.PushBack({ 1058, 412, 70,70 });
	deadanim.PushBack({ 1128, 412, 70,70 });
	deadanim.PushBack({ 1198, 412, 70,70 });
	deadanim.PushBack({ 1268, 412, 70,70 });
	deadanim.PushBack({ 1338, 412, 70,70 });
	deadanim.PushBack({ 918, 482, 70,70 });
	deadanim.PushBack({ 0,0,0,0 });
	deadanim.speed = 0.2f;
	deadanim.loop = false;

}

j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node&)
{
	return true;
}

bool j1Player::Start()
{
	LOG("Loading Player");

	position.x = spawn.x;
	position.y = spawn.y;
	sprite = App->tex->Load("textures/character.png");
	playerhitbox = App->collision->AddCollider({ 0, 0, 20, 30 }, COLLIDER_PLAYER, this);
	jump_state = REST;
	speedx = 5;
	speedy = 8;
	moveleft = true;
	moveright = true;
	jump_time = 0;
	doublejump = 2;
	deadcount = 0;
	App->render->camera.x = 0;
	current_animation = &idleright;

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading Player");
	App->collision->EraseCollider(playerhitbox);
	App->tex->UnLoad(sprite);
	return true;
}

bool j1Player::Load(pugi::xml_node& save) {

	if (save.child("pos") != NULL) {
		loadposition.x = save.child("pos").attribute("x").as_int();
		loadposition.y = save.child("pos").attribute("y").as_int();
	}

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const {
	

	if (data.child("pos") == NULL) {
		data.append_child("pos").append_attribute("x") = position.x;
		data.child("pos").append_attribute("y") = position.y;
	}
	else {
		data.child("pos").attribute("x").set_value(position.x);
		data.child("pos").attribute("y").set_value(position.y);
	}
	
	return true;
}

bool j1Player::Update(float dt) {
	//Input
	if (dead == false && change == false) {
			Movement();
			Jump();
			Gravity();
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
			position.x = spawn.x;
			position.y = spawn.y;
			App->render->camera.x = 0;
		}

	}



	//Collider

	playerhitbox->SetPos(position.x + 14, position.y + 25);

	


	if (position.x > 480 && position.x < 1760) {
		App->render->camera.x = 0 - (position.x * 2 - App->win->width / 2);
	}


	if (dead == true) {
		if (deadcount == 0) {
			deadanim.Reset();
			deadposition.x = position.x;
			deadposition.y = position.y;
			position.x = position.y = -100;
			deadcount++;
		}
		if (deadcount == 100) {
			deadcount = 0;
			dead = false;
			current_animation = &idleright;
			App->render->camera.x = 0;
			position.x = spawn.x;
			position.y = spawn.y;
		}
		else
		{
			current_animation = &deadanim;
			deadcount++;
			App->render->Blit(sprite, deadposition.x - 15, deadposition.y + 3, &(current_animation->GetCurrentFrame()));
		}
	}

	if(change == true)
	{
		Change();
	}
	//Draw
	App->render->Blit(sprite, position.x, position.y, &(current_animation->GetCurrentFrame()));


	return true;

}

void j1Player::Gravity() {
	iPoint jump;
	iPoint jump2;
	iPoint playerpos;
	jump.x = playerhitbox->rect.x;
	jump2.x = playerhitbox->rect.x + playerhitbox->rect.w;
	jump.y = playerhitbox->rect.y + playerhitbox->rect.h + speedy + 1;
	jump2.y = playerhitbox->rect.y;
	playerpos.y = playerhitbox->rect.y + playerhitbox->rect.h;




	if (((Check_Collision(jump.x, jump.y, 19) || Check_Collision(jump2.x, jump.y, 19) && jump_state != JUMP) || Check_Collision(jump.x, jump2.y, 19) || Check_Collision(jump2.x, jump2.y, 19) && jump_state != JUMP)){
		jump_state = REST;
		doublejump = 2;
		//position.y;
		if (current_animation == &planeleft || current_animation == &planeright) {
			current_animation = &idleright;
		}

	}
	else if (jump_state != JUMP) {
		jump_state = FALL;
	}
	if (jump_state == FALL) {

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && dead == false)
		{
			if (current_animation == &walkleft || current_animation == &idleleft || current_animation == &planeleft) {
				current_animation = &planeleft;
			}
			else {
				current_animation = &planeright;
			}

			speedy = 2;
		}
		else if (jump_state != JUMP) {
			speedy = 8;
		}
	}

	if (jump_state == JUMP) {
		position.y -= speedy;
	}
	if (jump_state == FALL) {
		position.y += speedy;
	}


}

void j1Player::Jump() {
	iPoint jumpup;
	iPoint jumpup2;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		jumping = true;
	}

	if (jumping == true) {
		if (doublejump == 2) {
			if (jump_time < 15) {
				jump_state = JUMP;
				jump_time++;
				speedy = 0;
				position.y -= 6;

				jumpup.x = playerhitbox->rect.x;
				jumpup2.x = playerhitbox->rect.x + playerhitbox->rect.w;
				jumpup.y = jumpup2.y = playerhitbox->rect.y - 6;


				if (Check_Collision(jumpup.x, jumpup.y, 19) || Check_Collision(jumpup2.x, jumpup.y, 19))
				{
					position.y += 6;
					jump_time = 45;
				}
				if (Check_Collision(jumpup.x, jumpup.y, 4) || Check_Collision(jumpup2.x, jumpup.y, 4)) 
				{
					dead = true;
				}
			}
			else {
				doublejump--;
				jump_time = 0;
				jump_state = FALL;
				jumping = false;
			}
		}
		else{
			if (jump_time < 10) {
				jump_state = JUMP;
				jump_time++;
				speedy = 0;
				position.y -= 8;
				jumpup.x = playerhitbox->rect.x;
				jumpup2.x = playerhitbox->rect.x + playerhitbox->rect.w - 1;
				jumpup.y = jumpup2.y = playerhitbox->rect.y - 12;

				if (Check_Collision (jumpup.x,jumpup.y,19) || Check_Collision(jumpup2.x, jumpup2.y, 19)) {
					position.y += 8;
					jump_time = 25;
				}
				if (Check_Collision(jumpup.x, jumpup.y, 4) || Check_Collision(jumpup2.x, jumpup2.y, 4)) {
					dead = true;
				}
			}
			else {
				doublejump--;
				jump_time = 0;
				jump_state = FALL;
				jumping = false;
			}

		}
	}
}


void j1Player::Movement() {

	//Need 2 points on vertical hitbox because player hight = 2 tiles

	iPoint up;
	iPoint down;
	iPoint middle;

	up.x = playerhitbox->rect.x - 7;
	up.y = playerhitbox->rect.y - speedy;

	middle.x = playerhitbox->rect.x + 7;
	middle.y = playerhitbox->rect.y + playerhitbox->rect.h;

	down.x = playerhitbox->rect.x + playerhitbox->rect.w + 7;
	down.y = playerhitbox->rect.y + playerhitbox->rect.h;



	// RIGHT

	if (Check_Collision(down.x, up.y, 19) || Check_Collision(down.x, middle.y, 19) || Check_Collision(down.x, down.y, 19)) 
	{
		moveright = false;
	}
	else 
	{
		moveright = true;
	}

	if (Check_Collision(down.x, up.y, 4) || Check_Collision(down.x, middle.y, 4) || Check_Collision(down.x, down.y, 4))
	{
		dead = true;
	}

	if (Check_Collision(down.x, up.y, 14) || Check_Collision(down.x, middle.y, 14) || Check_Collision(down.x, down.y, 14)) 
	{
		position.x = position.y = -100;
		change = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && moveright == true)
	{
		position.x += speedx;
		current_animation = &walkright;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) 
	{
		current_animation = &idleright;
	}

	//LEFT

	if(Check_Collision(up.x, up.y, 19) || Check_Collision(up.x, middle.y, 19) || Check_Collision(up.x, down.y, 19))
	{
		moveleft = false;
		position.x += speedx;
	}
	else
	{
		moveleft = true;
	}
	if (Check_Collision(up.x, up.y, 4) || Check_Collision(up.x, middle.y, 4) || Check_Collision(up.x, down.y, 4))
	{
		dead = true;
	}
	if (Check_Collision(up.x, up.y, 14) || Check_Collision(up.x, middle.y, 14) || Check_Collision(up.x, down.y, 14))
	{
		position.x = position.y = -100;
		change = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speedx;
		current_animation = &walkleft;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		current_animation = &idleleft;
	}
}

bool j1Player::Check_Collision(int x, int y, int id) {

	iPoint vec;

	vec = App->map->WorldToMap(x, y);

	if (App->map->logic_layer->data->Get(vec.x, vec.y) == id)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void j1Player::Change() {
 
	switch (App->scene->level)
	{
	case 1:
		App->scene->level = 2;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 3);
		change = false;
		break;
	case 0:
		App->scene->level = 1;
		App->fadetoblack->FadeToBlack((j1Module*)App->scene, (j1Module*)App->scene, 3);
		change = false;
		break;
	default:
		break;
	}
	

}