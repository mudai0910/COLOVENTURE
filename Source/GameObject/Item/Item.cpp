#include "Item.h"
#include <DxLib.h>

Item::Item()
	: graphics_handle(-1)
{
}

Item::~Item()
{
	Finalize();
}

void Item::Initialize()
{
	__super::Initialize();

}

void Item::Update(float dt)
{
	__super::Update(dt);
}

void Item::Draw(const Vector2D& screen_offset)
{
	if (graphics_handle == -1)
		return;

	DrawGraph(position.x - screen_offset.x, position.y - screen_offset.y, graphics_handle, true);
}

void Item::Finalize()
{
}

void Item::OnDestroy()
{
	__super::OnDestroy();
}
