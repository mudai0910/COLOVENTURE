#include "EnergyDrink.h"
#include "DxLib.h"
#include "../../../Manager/GraphicsManager/GraphicsManager.h"

EnergyDrink::EnergyDrink()
	: Item()
{
}

EnergyDrink::~EnergyDrink()
{
	Finalize();
}

void EnergyDrink::Initialize()
{
	__super::Initialize();

	object_type = EObjectType::ENERGYDRINK;

	graphics_handle = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/mapchip_033.bmp"));

	int size_x, size_y;
	GetGraphSize(graphics_handle, &size_x, &size_y);
	box_collision = Vector2D(size_x, size_y);
}

void EnergyDrink::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

}

void EnergyDrink::OnCollision(GameObject* object)
{
	if (object->GetObjectType() == EObjectType::PLAYER)
	{
		Destroy();
	}
}

