#include "j1Player.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Render.h"
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

	}
	load_anim->loop = attributes.attribute("loop").as_bool(true);
	load_anim->speed = attributes.attribute("speed").as_float();

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
	speed.x = 5; speed.y = 5;
	p.x = p.y = 100;
	p.w = 16; p.h = 60;
	test = App->tex->Load(animations.child("texture").child("folder").attribute("file").as_string());
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
		p.y -= speed.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		p.y += speed.y;
	}

	Draw();

	return ret;
}


void j1Player::Draw()
{
	App->render->DrawQuad(p, 0, 255, 0, 255);

	iPoint offset;
	offset = GetOffset(offset.x, offset.y);

	current_animation = &idle;
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(test, p.x - offset.x, p.y - offset.y, &r);


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


void j1Player::Right()
{
	p.x += speed.x;
}


void j1Player::Left()
{
	p.x -= speed.x;
}


bool j1Player::CheckCollision()
{
	bool ret = true;



	return ret;
}
