#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "UIElement.h"
#include "SDL\include\SDL.h"
#include "p2Defs.h"
#include "p2Point.h"

class Button : public UIElement
{
public:
	Button(int pos_x, int pos_y, uint type, SDL_Texture* texture, j1Module* callback, UIElement* parent = nullptr);
};



#endif // !_BUTTON_H__
