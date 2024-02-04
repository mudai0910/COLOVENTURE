#pragma once
#include "../../GameObject.h"

class BackGround : public GameObject
{
public:
	BackGround();
	virtual ~BackGround();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface
	//背景画像をロードするための関数
	void LoadGraphics(std::string background_graphic_path);
	void SetDistanceScale(const float new_distance_scale) { distance_scale = new_distance_scale; }

private:
	int background_graphic_handle;

	// (カメラ・背景平面の距離) / (カメラ・ステージ平面の距離)
	float distance_scale = 25.f;

	// 背景の繰り返し単位を3枚横に並べて描画する
	Vector2D left_panel_origin;
	Vector2D center_panel_origin;
	Vector2D right_panel_origin;

	//背景のスクリーンオフセット
	Vector2D background_screen_offset;

	void DrawBackGround(const Vector2D& screen_offset);
};
