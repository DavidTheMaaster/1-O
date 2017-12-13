#include "Image.h"
#include "j1App.h"
#include "j1Gui.h"

Image::Image(int x, int y, uint type, SDL_Texture * texture, Animation* anim, UIElement* parent) : UIElement(pos.x, pos.y, type, texture, parent)
{
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
