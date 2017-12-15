#include "Button.h"
#include "j1App.h"
#include "j1Gui.h"

Button::Button(int x, int y, uint type, SDL_Texture * texture, Animation* anim, j1Module* callback, UIElement* parent) : UIElement(pos.x, pos.y, type, texture, parent)
{
	this->callback = callback;

	this->parent = parent;

	if (this->parent != nullptr)
	{
		pos.x = this->parent->pos.x + x;
		pos.y = this->parent->pos.y + y;
	}
	else
	{
		pos.x = x;
		pos.y = y;
	}

	if (anim != nullptr)
		this->anim = *anim;

}
