#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2List.h"
#include "p2Point.h"


struct SDL_Texture;
struct Collider;

class j1Player : public j1Module
{

public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool CleanUp();
	bool Update(float dt);
	void Gravity();
	void Movement();
	void Jump();
	void Change();
	
	// Save & Load
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);


	bool Check_Collision(int x, int y, int id);



private:
	SDL_Texture* sprite = nullptr;
	Animation* current_animation = nullptr;

	Animation idleright;
	Animation idleleft;
	Animation walkright;
	Animation walkleft;
	Animation jumpleft;
	Animation jumpright;
	Animation planeright;
	Animation planeleft;
	Animation deadanim;


	Collider* playerhitbox;

	//Speeds
	

	//Movement
	bool moveleft;
	bool moveright;
	bool fall;
	bool canjump;
	uint jump_state;
	uint jump_time;
	uint jump_dir;
	bool jumping;
	uint doublejump;
	bool dead;
	bool change;

	uint deadcount;

	enum States
	{
		FALL,
		JUMP,
		REST
	};
	
public:
	//Spawn
	iPoint spawn;
	iPoint deadposition;
	iPoint position;
	iPoint loadposition;

	float speedx;
	float speedy;
};


#endif