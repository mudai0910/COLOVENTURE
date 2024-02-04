#pragma once

#include "../../GameObject.h"

class Ground : public GameObject
{
public:
	Ground();
	virtual ~Ground();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

	//マップデータのセット
	void SetMapData(std::vector<std::vector<std::string>> new_map_data) { map_data = new_map_data; }
	//マップデータのvector配列
	std::vector<std::vector<std::string>> map_data;
	//マップチップ画像ハンドル,最初の要素は0にする
	std::vector<int> mapchip_graphics_handles;

private:
	// GR = Ground(地面), SC = SCAFFOLD(足場)
	enum class EMapChipType : unsigned char
	{
		NONE_IMAGE,   // 0:画像無し
		GR_SURFACE_LEFT,	
		GR_SURFACE_CENTER,
		GR_SURFACE_RIGHT,
		GR_MIDDLE_LEFT,
		GR_MIDDLE_CENTER,
		GR_MIDDLE_RIGHT,
		GR_BOTTOM_LEFT,
		GR_BOTTOM_CENTER,
		GR_BOTTOM_RIGHT,  

		SC_IRON_L,	// 10
		SC_WOOD_1_L,
		SC_WOOD_2_L,
		SC_LOG_L,
		SC_IRON_S,
		SC_WOOD_1_S,
		SC_WOOD_2_S,
		SC_LOG_S,

		MAX
	};
};
