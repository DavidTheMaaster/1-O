#include "j1Player.h"
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

j1Player::j1Player()
{
	name.create("player");
}

j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node& config)
{

	LOG("Loading animations");

	bool ret = true;

	animation_file.load_file(config.child("file").attribute("folder").as_string());
	animations = animation_file.child("animations").child("player").first_child();

	if (animations == NULL)
	{
		LOG("Could not load animations");
		ret = false;
	}

	while(animations != NULL){
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

	return ret;
}


bool j1Player::Start()
{
	bool ret = true;
	LOG("Loading player");
	speed.x = 4; speed.y = 8;
	p.x = 130; p.y = 100;
	p.w = 16; p.h = 56;
	current_animation = &idle;
	jump_counter = 0;
	flip = false;
	jumps = 2;
	texture = App->tex->Load(animations.child("texture").child("folder").attribute("file").as_string());
	return ret;
}

bool j1Player::CleanUp()
{
	bool ret = true;
	LOG("Unloading player");

	App->tex->UnLoad(texture);
	return ret;
}

bool j1Player::Update(float dt)
{
	bool ret = true;
	
	current_animation = &idle;

	Movement();
	CameraMovement();
	Gravity();
	Draw();

	//DEBUG


	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) 
	{
		p.x = p.y = 100;
		App->render->camera.x = 0;
	}


	return ret;
}


void j1Player::Draw()
{
	if ((App->scene->level == 0 || App->scene->level == 1) && App->fadetoblack->IsFading()==false)
	{
		iPoint offset;
		offset = GetOffset(offset.x, offset.y);

		SDL_Rect r = current_animation->GetCurrentFrame();

		App->render->DrawQuad(p, 0, 255, 0, 255);
		App->render->Blit(texture, p.x - offset.x, p.y - offset.y, &r, flip);
	}

}

void j1Player::Movement()
{
	if ((App->scene->level == 0 || App->scene->level == 1) && App->fadetoblack->IsFading() == false)
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
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
			speed.y = 8;
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

void j1Player::Right()
{
	if (App->collision->CheckCollisionRight(p))
	{
		p.x += speed.x;
		flip = false;
		current_animation = &walk;
	}
}


void j1Player::Left()
{
	if(App->collision->CheckCollisionLeft(p))
	{
		p.x -= speed.x;
		flip = true;
		current_animation = &walk;
	}
}



void j1Player::Gravity()
{
	if (App->collision->CheckCollisionDown(p) && jump == false && jump2 == false)
	{
		p.y += speed.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && CanDoubleJump()) {
			jump2 = true;
			jump_counter = 0;
			jump = false;
	}
	if (App->collision->CheckCollisionDown(p) == false)
	{
		jumps = 2;
	}
}

void j1Player::Jump()
{
	if (App->collision->CheckCollisionUp(p) && jump2 == false) {
		speed.x = 6;
		speed.y = 8;
		jumps = 1;
		if (jump_counter < 12)
		{
			p.y -= speed.y;
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
			speed.x = 4;
		}
	}
	else 
	{
		jump_counter = 0;
		jump = false;
		speed.x = 4;
		jump = false;
	}
}

bool j1Player::CanJump()
{
	bool ret = true; 
	if (jump == true)
	{
		ret  = false;
	}
	
	if (App->collision->CheckCollisionDown(p) && jump == false)
	{
		ret = false;
	}
	if (jumps == 0)
	{
		ret = false;
	}
	return ret;
}

void j1Player::DoubleJump()
{
	if (App->collision->CheckCollisionUp(p))
	{
		jumps = 0;
		if (jump_counter < 7)
		{
			p.y -= speed.y;
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
			speed.x = 4;
		}
	}
	else 
	{
		jump2 = false;
		jump_counter = 0;
		jump2 = false;
		speed.x = 4;
	}
	
}

bool j1Player::CanDoubleJump()
{
	bool ret = false;
	if(App->collision->CheckCollisionDown(p))
	{
		ret = true;
	}
	if (jumps == 0)
	{
		ret = false;
	}
	return ret;
}

void j1Player::Hover()
{
	speed.y = 4;

	if (App->collision->CheckCollisionDown(p)==false)
	{
		speed.y = 8;
	}
	else
	{
		current_animation = &hover;
	}
}

void j1Player::CameraMovement()
{

	if (p.x > App->win->width / 4 && p.x < 1760) {

		App->render->camera.x = 0 - (p.x * 2 - App->win->width / 2);

	}
}

iPoint j1Player::GetOffset(int x, int y)
{
	iPoint offset;

	if (current_animation == &idle) {
		x = animations.child("idle").child("attributes").attribute("offset_x").as_int(0);
		y = animations.child("idle").child("attributes").attribute("offset_y").as_int(0);
	}

	if (current_animation == &walk) {
		x = animations.child("walk").child("attributes").attribute("offset_x").as_int(0);
		y = animations.child("walk").child("attributes").attribute("offset_y").as_int(0);
	}

	if (current_animation == &hover) {
		x = animations.child("hover").child("attributes").attribute("offset_x").as_int(0);
		y = animations.child("hover").child("attributes").attribute("offset_y").as_int(0);
	}



	offset.x = x;
	offset.y = y;

	return iPoint(offset);
}




