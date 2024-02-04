#include "Coin.h"
#include "DxLib.h"
#include "../../../Manager/GraphicsManager/GraphicsManager.h"

Coin::Coin()
	: Item()
{
}

Coin::~Coin()
{
	Finalize();
}

void Coin::Initialize()
{
	__super::Initialize();

	object_type = EObjectType::COIN;

	graphics_handle = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/mapchip_035.bmp"));

	int size_x, size_y;
	GetGraphSize(graphics_handle, &size_x, &size_y);
	box_collision = Vector2D(size_x, size_y);
}

void Coin::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

}

void Coin::OnCollision(GameObject* object)
{
	if (object->GetObjectType() == EObjectType::PLAYER)
	{
		Destroy();
	}
}
