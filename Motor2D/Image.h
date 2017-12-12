#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "UIElement.h"
#include "SDL\include\SDL.h"
#include "p2Defs.h"
#include "p2Point.h"
#include "Animation.h"

class Image : public UIElement
{
public:
	Image(int pos_x, int pos_y, uint type, SDL_Texture* texture, Animation* anim = NULL, UIElement* parent = nullptr);
};



#endif // !_LABEL_H__

