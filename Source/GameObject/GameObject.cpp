#include "GameObject.h"
#include "../Scene/SceneBase.h"
#include "../SystemTypes.h"

GameObject::GameObject()
{
	owner_scene = nullptr;
	position = Vector2D();
	velocity = Vector2D();
	is_delete = false;
	object_type = EObjectType::NONE;
}

GameObject::~GameObject()
{
	Finalize();
}

void GameObject::Destroy()
{
	OnDestroy();
}

void GameObject::OnDestroy()
{
	SetPosition(Vector2D(SCREEN_RESOLUTION_X,SCREEN_RESOLUTION_Y));
}

void GameObject::Respawn(Vector2D respawn_point)
{
	SetPosition(respawn_point);
}