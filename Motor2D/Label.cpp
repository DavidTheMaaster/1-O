#include "Label.h"
#include "j1App.h"
#include "j1Gui.h"

Label::Label(int pos_x, int pos_y, uint type, const SDL_Texture * texture, UIElement* parent) : UIElement(pos.x, pos.y, type, texture, parent)
{
	this->parent = parent;

	this->parent = parent;

	if (this->parent != nullptr)
	{
		pos.x = this->parent->pos.x + pos_x;
		pos.y = this->parent->pos.y + pos_y;
	}
	else
	{
		pos.x = pos_x;
		pos.y = pos_y;
	}
}

