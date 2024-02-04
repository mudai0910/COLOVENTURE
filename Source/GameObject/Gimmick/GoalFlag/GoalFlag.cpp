#include "GoalFlag.h"
#include "DxLib.h"
#include "../../../Manager/GraphicsManager/GraphicsManager.h"


GoalFlag::GoalFlag()
	: Gimmick()
{
	graphics_handle = 0;
}

GoalFlag::~GoalFlag()
{
	Finalize();
}

void GoalFlag::Initialize()
{
	__super::Initialize();

	object_type = EObjectType::GOALFLAG;

	graphics_handle = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/mapchip_046.bmp"));

	int size_x, size_y;
	GetGraphSize(graphics_handle, &size_x, &size_y);
	box_collision = Vector2D(size_x, size_y);
}

void GoalFlag::Draw(const Vector2D& screen_offset)
{
	DrawGraph(position.x - screen_offset.x, position.y - screen_offset.y, graphics_handle, true);

}

void GoalFlag::OnCollision(GameObject* object)
{
	if (object->GetObjectType() == EObjectType::PLAYER)
	{
		Destroy();
	}
}
