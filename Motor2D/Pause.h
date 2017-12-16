#ifndef _PAUSE_H__
#define _PAUSE_H__

#include "j1Module.h"

class Pause : public j1Module
{
public:

	Pause();
	virtual	~Pause();

	bool Awake(pugi::xml_node& node);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void LoadPause();
	void UnloadPause();
	void PauseButtons();

private:

};


#endif // !_PAUSE_H__

