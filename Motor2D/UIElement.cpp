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
	if (parent != nullptr)
		App->render->Blit(texture, parent->pos.x + pos.x, parent->pos.y + pos.y, &rect, false, 0.0);
	else
		App->render->Blit(texture, pos.x, pos.y, &rect, false, 0.0);
}

void UIElement::Update(float dt)
{
	if (type == BUTTON) {

		int mouse_x = 0, mouse_y = 0;
		App->input->GetMousePosition(mouse_x, mouse_y);

		if ((mouse_x > this->pos.x && mouse_x < this->pos.x + rect.w) && (mouse_y > this->pos.y && mouse_y < this->pos.y + rect.h) && state != L_MOUSE_PRESSED) {
			state = UI_STATE::MOUSE_ENTER;
			this->callback->UIEvent(this, state);
			LOG("Mouse Enter");
		}
		else if (state == UI_STATE::MOUSE_ENTER) {
			state = UI_STATE::MOUSE_LEAVE;
			this->callback->UIEvent(this, state);
			LOG("Mouse Leave");
		}
		else if (state == MOUSE_LEAVE)
			state = NO_STATE;

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && state == MOUSE_ENTER) {
			state = L_MOUSE_PRESSED;
			this->callback->UIEvent(this, state);
			SDL_Texture* cross; 
			cross = App->tex->Load("maps/cross.png");
			LOG("Mouse Left Click");
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && state == L_MOUSE_PRESSED) {
			state = NO_STATE;
			this->callback->UIEvent(this, state);
			LOG("Mouse Stop Left Click");
		}

		if (state == L_MOUSE_PRESSED)
		{
			if (mouse_x != mouse2.x || mouse_y != mouse2.y) {
				App->input->GetMouseMotion(mouse_movement.x, mouse_movement.y);
				pos.x += mouse_movement.x;
				pos.y += mouse_movement.y;
				mouse2.x = mouse_x;
				mouse2.y = mouse_y;
			}
		}
	}
}

