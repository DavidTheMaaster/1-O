#ifndef _UIElement_
#define _UIElement_

#include "SDL/include/SDL.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"



enum UI_TYPE
{
	NO_UI_TYPE,
	LABEL,
	BUTTON,
	IMAGE
};

class UIElement
{

public:

	SDL_Rect rect;
	iPoint pos;
	uint w;
	uint h;


	j1Module* callback = nullptr;
	UIElement* parent = nullptr;

	iPoint mouse_movement;
	iPoint mouse2;

	uint type;

	

	const SDL_Texture *texture = nullptr;

public:

	UIElement(int x, int y, uint type, const SDL_Texture* texture, UIElement* parent = nullptr);
	virtual ~UIElement() { };
	UI_STATE state;
	virtual void Draw(float dt);
	virtual void Update(float dt);

public:
	Animation anim;

};

#endif

