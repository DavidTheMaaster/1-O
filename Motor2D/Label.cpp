#include "Label.h"
#include "j1App.h"
#include "j1Gui.h"

Label::Label(int pos_x, int pos_y, uint type, const SDL_Texture * texture, UIElement* parent) : UIElement(pos.x, pos.y, type, texture, parent)
{
	pos.x = pos_x;
	pos.y = pos_y;

	this->parent = parent;
}

