#include "j1Player.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Map.h"

j1Player::j1Player()
{
}

j1Player::~j1Player()
{
}

bool j1Player::Start()
{
	bool ret = true;
	LOG("Loading player");
	speed.x = 5; speed.y = 5;
	r.x = r.y = 100;
	r.w = 25; r.h = 50;

	return ret;
}

bool j1Player::CleanUp()
{
	bool ret = true;
	LOG("Unloading player");


	return ret;
}

bool j1Player::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		Right();
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		Left();
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		r.y -= speed.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		r.y += speed.y;
	}

	Draw();

	return ret;
}


void j1Player::Draw()
{
	App->render->DrawQuad(r, 0, 255, 0, 255);
}


void j1Player::Right()
{
	r.x += speed.x;
}


void j1Player::Left()
{
	r.x -= speed.x;
}


bool j1Player::CheckCollision()
{
	bool ret = true;



	return ret;
}
