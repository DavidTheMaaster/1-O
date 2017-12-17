#ifndef __J1PARTICLES_H__
#define __J1PARTICLES_H__


#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "j1Collision.h"

#define MAX_ACTIVE_PARTICLES 1000

struct SDL_Texture;
struct Collider;
enum COLLIDER_TYPE;

enum Particles
{
	SHOOT,
	EXPLOSION,
};

struct Particle
{
	Collider* collider = nullptr;
	Animation anim;
	uint fx = 0;
	fPoint position;
	fPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	bool fx_played = false;
	bool flip = false;
	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
};

class j1Particles : public j1Module
{
public:
	j1Particles();
	~j1Particles();

	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	void AddParticle(const Particle& particle, int x, int y, float speed_x, float speed_y, COLLIDER_TYPE collider_type = COLLIDER_NONE, bool fliped = false ,Uint32 delay = 0);

private:

	Particle* active[MAX_ACTIVE_PARTICLES];
	uint last_particle = 0;

	SDL_Texture* particles_graphics = nullptr;

public:

	Particle shoot;
	Particle explosion;

private:
	pugi::xml_document	animation_file;
	pugi::xml_node animations;
	pugi::xml_node attributes;
	pugi::xml_node rect;

	uint current = 0;
	Particle* load_particle = nullptr;

	uint anim_speed[2];


	void UpdateSpeed(float dt);
};

#endif // __MODULEPARTICLES_H__