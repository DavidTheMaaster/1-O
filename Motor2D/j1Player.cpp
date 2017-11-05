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
	current = attributes.attribute("name").as_string();
	rect = animations.first_child();

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

	return ret;
}


bool j1Player::Start()
{
	bool ret = true;
	LOG("Loading player");
	speed.x = 5; speed.y = 5;
	p.x = p.y = 100;
	p.w = 25; p.h = 50;
	test = App->tex->Load("textures/animations.png");
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

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		App->render->Blit(test, p.x, p.y, &(walk.GetCurrentFrame()));
	}
	else {
		App->render->Blit(test, p.x, p.y, &(idle.GetCurrentFrame()));
	}
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
