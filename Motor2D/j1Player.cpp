#include "j1Player.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Map.h"

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

	animation_file.load_file(config.child("file").attribute("direction").as_string());
	animations = animation_file.child("animations").first_child();

	if (animations == NULL)
	{
		LOG("Could not load animations");
		ret = false;
	}

	while(animations != NULL){
	attributes = animations.child("attributes");
	rect = animations.first_child();

	current = attributes.attribute("name").as_string();

	if (current == "idle")
	{
		load_anim = &idle;
	}
	else if (current == "walk")
	{
		load_anim = &walk;
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
		animations = animation_file.child("animations");
	}

	return ret;
}


bool j1Player::Start()
{
	bool ret = true;
	LOG("Loading player");
	speed.x = 4; speed.y = 8;
	jumplimit = 20;
	p.x = p.y = 100;
	p.w = 16; p.h = 56;
	current_animation = &idle;
	flip = false;
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

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		Right();
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		Left();
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && jumping == false) {
		Jump();
		canjump2 = true;
		//jumping = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) {

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && canjump2 == true) {
			Jump2();
			canjump2 = false;
			jumping = false;
		}

	}

	Gravity();
	if (App->collision->CheckCollisionDown(p) == false) {
		jumping = false;
	}
	Draw();

	return ret;
}


void j1Player::Draw()
{
	iPoint offset;
	offset = GetOffset(offset.x, offset.y);

	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(texture, p.x - offset.x, p.y - offset.y, &r, flip);

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

void j1Player::Jump()
{
	if (App->collision->CheckCollisionUp(p))
	{	
		if (jumplimit >= 0) {
			p.y -= speed.y + jumplimit;
			//current_animation = &jump;
			jumplimit--;
		}
		else if (jumping){
			jumplimit = 20;
			canjump2 = true;
		}
	}
}


void j1Player::Jump2()
{
	if (App->collision->CheckCollisionUp(p))
	{
		if (jumplimit >= 0 && canjump2 == true) {
			p.y -= speed.y + jumplimit;
			//current_animation = &jump;
			jumplimit--;
		}
		else {
			jumplimit = 20;
			canjump2 = false;
		}
	}
}

void j1Player::Gravity()
{
	if (App->collision->CheckCollisionDown(p))
	{
		p.y += speed.y;
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


	offset.x = x;
	offset.y = y;

	return iPoint(offset);
}



