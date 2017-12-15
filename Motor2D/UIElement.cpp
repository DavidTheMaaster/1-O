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
	}
}

