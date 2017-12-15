#ifndef _PAUSE_H__
#define _PAUSE_H__

#include "j1Module.h"
#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "Animation.h"

struct SDL_Texture;

class Pause : public j1Module
{
public:
	Pause();
	virtual ~Pause();
	bool Awake(pugi::xml_node& node);
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void LoadPauseUI();

private:

};


#endif