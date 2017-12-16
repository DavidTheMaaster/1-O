#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "UIElement.h"
#include "SDL\include\SDL.h"
#include "p2Defs.h"
#include "p2Point.h"

class Slider : public UIElement
{
public:
	Slider(int pos_x, int pos_y, uint type, SDL_Texture* texture, Animation* anim, j1Module* callback, UIElement* parent = nullptr);
};



#endif // !_SLIDER_H__

