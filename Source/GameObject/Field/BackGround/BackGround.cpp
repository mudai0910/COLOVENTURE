#include "BackGround.h"
#include <DxLib.h>
#include <string>
#include "../../../SystemTypes.h"
#include "../Source/Scene/SceneBase.h"
#include "../../../Manager/GraphicsManager/GraphicsManager.h"


BackGround::BackGround()
{
	background_graphic_handle = 0;
}

BackGround::~BackGround()
{
}

void BackGround::Initialize()
{
	__super::Initialize();

	background_screen_offset = Vector2D(0.f, 0.f);
	left_panel_origin = Vector2D(-SCREEN_RESOLUTION_X, 0);
	center_panel_origin = Vector2D(0, 0);
	right_panel_origin = Vector2D(SCREEN_RESOLUTION_X, 0);

}

void BackGround::LoadGraphics(std::string background_graphic_path)
{
	background_graphic_handle = GraphicsManager::GetInstance().LoadGraph(_T(background_graphic_path));
}


void BackGround::Update(float delta_seconds)
{
	__super::Update(delta_seconds);

	if (background_screen_offset.x < center_panel_origin.x - SCREEN_RESOLUTION_X / 2)
	{
		right_panel_origin.x -= SCREEN_RESOLUTION_X;
		center_panel_origin.x -= SCREEN_RESOLUTION_X;
		left_panel_origin.x -= SCREEN_RESOLUTION_X;
	}
	else if (background_screen_offset.x >= center_panel_origin.x + SCREEN_RESOLUTION_X / 2)
	{
		right_panel_origin.x += SCREEN_RESOLUTION_X;
		center_panel_origin.x += SCREEN_RESOLUTION_X;
		left_panel_origin.x += SCREEN_RESOLUTION_X;
	}


}

void BackGround::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	//背景のスクリーンオフセットの更新(screen_offsetを使用するためDraw関数内に記述）
	background_screen_offset = screen_offset / distance_scale;
	//背景を描画
	DrawBackGround(screen_offset);

}

void BackGround::Finalize()
{
	__super::Finalize();
}
//背景の描画
void BackGround::DrawBackGround(const Vector2D& screen_offset)
{
	Vector2D draw_left_from = left_panel_origin - background_screen_offset;
	Vector2D draw_to = draw_left_from + Vector2D(SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y);
	int x_left_from, y_left_from, x_left_to, y_left_to;
	draw_left_from.ToInt(x_left_from, y_left_from);
	draw_to.ToInt(x_left_to, y_left_to);

	Vector2D draw_center_from = center_panel_origin - background_screen_offset;
	Vector2D draw_center_to = draw_center_from + Vector2D(SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y);
	int x_center_from, y_center_from, x_center_to, y_center_to;
	draw_center_from.ToInt(x_center_from, y_center_from);
	draw_center_to.ToInt(x_center_to, y_center_to);

	Vector2D draw_right_from = right_panel_origin - background_screen_offset;
	Vector2D draw_right_to = draw_right_from + Vector2D(SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y);
	int x_right_from, y_right_from, x_right_to, y_right_to;
	draw_right_from.ToInt(x_right_from, y_right_from);
	draw_right_to.ToInt(x_right_to, y_right_to);

	DrawExtendGraph(x_left_from, y_left_from, x_left_to, y_left_to, background_graphic_handle, true);
	DrawExtendGraph(x_center_from, y_center_from, x_center_to, y_center_to, background_graphic_handle, true);
	DrawExtendGraph(x_right_from, y_right_from, x_right_to, y_right_to, background_graphic_handle, true);

	DrawLine(x_left_from, y_left_from, x_left_from, y_left_from + 100, GetColor(255, 0, 0));
}

