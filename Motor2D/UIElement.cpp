#include "j1Render.h"
#include "j1App.h"
#include "j1Gui.h"
#include "UIElement.h"
#include "j1Input.h"
#include "p2Log.h"

UIElement::UIElement(int x, int y, uint type, const SDL_Texture* texture, UIElement* parent)
{
	this->texture = texture;
	App->tex->GetSize(texture, w, h);

	this->type = type;

	pos.x = x;
	pos.y = y;

	rect = { x, y, (int)w, (int)h };
}

void UIElement::Draw(float dt)
{

	if (anim.GetCurrentFrame().w == 0)
		current_animation = rect;
	else
		current_animation = anim.GetCurrentFrame();

	App->render->Blit(texture, pos.x, pos.y, &(current_animation), false, 0.0);

}

void UIElement::Update(float dt)
{
	SDL_Rect button_rect;

	if (current_animation.w == 0)
		button_rect = rect;
	else
		button_rect = current_animation;

	if (type == BUTTON) {

		int mouse_x = 0, mouse_y = 0;
		App->input->GetMousePosition(mouse_x, mouse_y);

		if (state == MOUSE_ENTER) {
			state = FOCUSED;
		}

		if ((mouse_x > this->pos.x && mouse_x < this->pos.x + button_rect.w) && (mouse_y > this->pos.y && mouse_y < this->pos.y + button_rect.h) && state != L_MOUSE_PRESSED && state != FOCUSED) {
			state = MOUSE_ENTER;
			this->callback->UIEvent(this, state);
			LOG("Mouse Enter");
		}
		else if (state == FOCUSED && ((mouse_x < this->pos.x || mouse_x > this->pos.x + button_rect.w) || (mouse_y < this->pos.y || mouse_y > this->pos.y + button_rect.h))) {
			state = MOUSE_LEAVE;
			this->callback->UIEvent(this, state);
			LOG("Mouse Leave");
		}
		else if (state == MOUSE_LEAVE)
			state = NO_STATE;

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && state == FOCUSED) {
			state = L_MOUSE_PRESSED;
			this->callback->UIEvent(this, state);
			LOG("Mouse Left Click");
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && state == L_MOUSE_PRESSED) {
			state = NO_STATE;
			this->callback->UIEvent(this, state);
			LOG("Mouse Stop Left Click");
		}

		if (state == L_MOUSE_PRESSED)
		{
			App->input->GetMouseMotion(mouse_x, mouse_y);

			if (parent != nullptr && parent->type == SLIDER)
			{
				if (mouse_x != mouse2.x)
				{
					int xRight = pos.x + rect.w + mouse_x;
					int xLeft = pos.x + mouse_x;

					if (xRight <= parent->pos.x + parent->rect.w
						&& xLeft >= parent->pos.x)
					{
						pos.x += mouse_x;
					}
					else
					{
						if (xRight > parent->pos.x + parent->rect.w)
						{
							pos.x = parent->rect.w - rect.w;
						}
						else
						{
							pos.x = parent->pos.x;
						}
					}
					mouse2.x = mouse_x;
					mouse2.y = mouse_y;
				}
			}
			else if (mouse_x != mouse2.x || mouse_y != mouse2.y)
			{
				pos.x += mouse_x;
				pos.y += mouse_y;
				mouse2.x = mouse_x;
				mouse2.y = mouse_y;
			}
			
			slider_value = GetSliderValue(this, parent);
		}

	}
	
}

void UIElement::ChangeLabel(char* text, uint colors, uint fonts, int size)
{
	SDL_Color color;

	color = App->gui->GetColor(colors);
	char* path = App->gui->GetFont(fonts);
	_TTF_Font* font = App->font->Load(path, size);

	const SDL_Texture* tex = App->font->Print(text, color, font);

	texture = tex;
}

int UIElement::GetSliderValue(UIElement* zap, UIElement* slider)
{
	return zap->pos.x * 100 / slider->rect.w - zap->rect.w;
}