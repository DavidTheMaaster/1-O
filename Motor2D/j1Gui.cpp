#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "Slider.h"

#include "Brofiler/Brofiler.h"


j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	BROFILER_CATEGORY("GUI Update", Profiler::Color::CornflowerBlue);
	p2List_item<UIElement*> *it = elements.start;
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		debug = !debug;
	}


	while (it != nullptr)
	{
		it->data->Draw(dt);
		it->data->Update(dt);
		it = it->next;
	}

	return true;

}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UIElement*>* item;
	item =elements.start;

	while (item != NULL)
	{
		App->tex->UnLoad((SDL_Texture*)item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	elements.clear();

	return true;
}


// class Gui ---------------------------------------------------

Label* j1Gui::AddLabel(int x, int y, char* text, uint colors, uint fonts, int size, UIElement* parent, Uint32 wrap)
{
	SDL_Color color;

	color = GetColor(colors);
	char* path = GetFont(fonts);
	_TTF_Font* font = App->font->Load(path, size);


	const SDL_Texture* tex = App->font->Print(text, color, font, wrap);

	Label* label = new Label(x, y, LABEL, tex, parent);
	elements.add((UIElement*)label);
	label->font = font;

	return label;
}


Image* j1Gui::AddImage(int x, int y, SDL_Texture* texture, Animation anim, UIElement* parent)
{
	Animation* animation = &anim;
	Image* image = new Image(x, y, IMAGE, texture, animation, parent);
	elements.add((UIElement*)image);

	return image;
}

Button* j1Gui::AddButton(int x, int y, SDL_Texture* texture, Animation anim, j1Module* callback, UIElement* parent)
{
	Animation* animation = &anim;
	Button* button = new Button(x, y, BUTTON, texture, animation, callback, parent);
	elements.add((UIElement*)button);


	return button;
}

Slider* j1Gui::AddSlider(int x, int y, SDL_Texture* texture, Animation anim, j1Module* callback, UIElement* parent)
{
	Animation* animation = &anim;
	Slider* slider = new Slider(x, y, SLIDER, texture, animation, callback, parent);
	elements.add((UIElement*)slider);


	return slider;
}

void j1Gui::DeleteUI(UIElement * element)
{
	int index = elements.find(element);
	if (index > -1)
		elements.del(elements.At(index));
}



SDL_Color j1Gui::GetColor(int color)
{
	SDL_Color ret;
	ret.a = 255;

	switch (color)
	{
	case VERY_BLACK:
		ret.r = 0;
		ret.g = 0;
		ret.b = 0;
		break;
	case BLACK:
		ret.r = 30;
		ret.g = 30;
		ret.b = 30;
		break;
	case WHITE:
		ret.r = 255;
		ret.g = 255;
		ret.b = 255;
		break;
	case RED:
		ret.r = 255;
		ret.g = 0;
		ret.b = 0;
		break;
	case BLUE:
		ret.r = 0;
		ret.g = 0;
		ret.b = 255;
		break;
	case GREEN:
		ret.r = 0;
		ret.g = 255;
		ret.b = 0;
		break;
	case YELLOW:
		ret.r = 218;
		ret.g = 202;
		ret.b = 124;
		break;
	default:
		break;
	}
	return ret;
}

char * j1Gui::GetFont(uint font)
{
	char* path = nullptr;
	switch (font)
	{
	case FREEPIXEL:
		path = "fonts/FreePixel.ttf";
		break;
	case FIPPS:
		path = "fonts/Fipps.ttf";
		break;
	case MINECRAFT:
		path = "fonts/Minecraft.ttf";
		break;
	case UPHEAVAL:
		path = "fonts/Upheaval.ttf";
		break;
	case ARIAL:
		path = "fonts/Arial.ttf";
		break;
	default:
		break;
	}
	return path;
}
