#include "Button.h"
#include "j1App.h"
#include "j1Gui.h"

Button::Button(int x, int y, uint type, SDL_Texture * texture, j1Module* callback, UIElement* parent) : UIElement(pos.x, pos.y, type, texture, parent)
{
	pos.x = x;
	pos.y = y;

	this->callback = callback;

	this->parent = parent;
}
