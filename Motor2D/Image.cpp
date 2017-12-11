#include "Image.h"
#include "j1App.h"
#include "j1Gui.h"

Image::Image(int x, int y, uint type, SDL_Texture * texture, UIElement* parent) : UIElement(pos.x, pos.y, type, texture, parent)
{
	pos.x = x;
	pos.y = y;

	this->parent = parent;
}
