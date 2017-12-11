#ifndef _LABEL_H_
#define _LABEL_H_

#include "UIElement.h"
#include "SDL\include\SDL.h"
#include "p2Defs.h"
#include "p2Point.h"

class Label : public UIElement
{
public:
	Label(int pos_x, int pos_y, uint type, const SDL_Texture* texture, UIElement* parent = nullptr);
};



#endif // !_LABEL_H__
