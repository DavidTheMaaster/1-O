#include "j1App.h"
#include "Entity.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Audio.h"

Entity::Entity(int x, int y) : position(x, y)
{}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;


}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);


	if (flying_enemy_animation != nullptr) {
		App->render->Blit(sprites, r.x, r.y, &(flying_enemy_animation->GetCurrentFrame()));
	}

	if (walking_enemy_animation != nullptr) {
		App->render->Blit(sprites, r.x, r.y, &(walking_enemy_animation->GetCurrentFrame()));
	}


}

void Entity::OnCollision(Collider* collider)
{

}