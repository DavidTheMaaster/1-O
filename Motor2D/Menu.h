#ifndef _MENU_H__
#define _MENU_H__

#include "j1Module.h"
#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "Animation.h"

struct SDL_Texture;

class Menu : public j1Module
{
public:
	Menu();
	~Menu();
	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	void GetKeys();
	void ButtonInteractions();

	bool Animations();
	void HandAnimation();

	void ResetMenu();



	void LoadMenuUI();
	void LoadOptionUI();
	void LoadLevelUI();
	void LoadPauseUI();

	void Pause();
	void MenuButtons();
	void OptionButtons();

	void SetVolume();


private:
	SDL_Texture *menu_texture, *ui_texture, *cross_texture;
	uint button_focused_fx;
	uint cross_click_fx;


};



#endif // !_MENU_H__

