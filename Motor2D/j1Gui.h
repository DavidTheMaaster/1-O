#ifndef _j1GUI_H__
#define _j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "UIElement.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2

class Label;
class Image;
class Button;
class Slider;

enum COLOURS
{
	VERY_BLACK,
	BLACK,
	WHITE,
	RED,
	BLUE,
	GREEN,
	YELLOW
};

enum UI_STATE {
	NO_STATE,
	MOUSE_ENTER,
	MOUSE_LEAVE,
	L_MOUSE_PRESSED,
	R_MOUSE_PRESSED,
	FOCUSED,
};

enum FONTS
{
	NO_FONT,
	FIPPS,
	FREEPIXEL,
	MINECRAFT,
	UPHEAVAL,
	ARIAL,
};

class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	const SDL_Texture* GetAtlas() const;

	// Gui creation functions
	bool Update(float dt);


	Label* AddLabel(int x, int y, char* text, uint color, uint font, int size, UIElement* parent = nullptr, Uint32 wrap = 100);
	Image* AddImage(int x, int y, SDL_Texture* texture, Animation anim = {}, UIElement* parent = nullptr);
	Button* AddButton(int x, int y, SDL_Texture* texture, Animation anim = {}, j1Module* callback = nullptr, UIElement* parent = nullptr);
	Slider* AddSlider(int x, int y, SDL_Texture* texture, Animation anim = {}, j1Module* callback = nullptr, UIElement* parent = nullptr);

	void DeleteUI(UIElement* element);


	SDL_Color GetColor(int color);
	char* GetFont(uint font);
	bool debug = false;
private:
	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<UIElement*> elements;


};

#endif // !_j1GUI_H__

