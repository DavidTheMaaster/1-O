#include "j1App.h"
#include "Entity.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Audio.h"

Entity::Entity(int x, int y) : position(x, y)
{}

Entity::~Entity()
{


}

void Entity::Draw(SDL_Texture* sprites)
{
	if (flying_enemy_animation != nullptr) {
		App->render->Blit(sprites, r.x - offset.x, r.y - offset.y, &(flying_enemy_animation->GetCurrentFrame()),flip);
	}

	if (walking_enemy_animation != nullptr) {
		App->render->Blit(sprites, r.x - offset.x - 17 , r.y - offset.y - 25, &(walking_enemy_animation->GetCurrentFrame()), flip);
	}

	if (player_animation != nullptr) {
		App->render->Blit(sprites, r.x - offset.x, r.y - offset.y, &(player_animation->GetCurrentFrame()), flip);
	}
	if (urn_animation != nullptr) {
		App->render->Blit(sprites, r.x - offset.x, r.y - offset.y, &(flying_enemy_animation->GetCurrentFrame()), flip);
	}
}

void Entity::OnCollision(Collider* collider)
{

}