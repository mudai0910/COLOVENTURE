#include "Ground.h"

#include "DxLib.h"
#include "../../../SystemTypes.h"


//コンストラクタ
Ground::Ground()
	:mapchip_graphics_handles{}
{

}
//デストラクタ
Ground::~Ground()
{
	Finalize();
}

void Ground::Initialize()
{
	__super::Initialize();

	//グラフィックハンドルの配列のサイズを確保
	mapchip_graphics_handles.resize((int)EMapChipType::MAX);

	// マップチップ画像の読み込み
	LoadDivGraph(_T("Resources/Images/mapchip_surface.bmp"),
		3, 3, 1,
		32, 48,
		&mapchip_graphics_handles[(int)EMapChipType::GR_SURFACE_LEFT]);
	LoadDivGraph(_T("Resources/Images/mapchip_under.bmp"),
		6, 3, 2,
		32, 32,
		&mapchip_graphics_handles[(int)EMapChipType::GR_MIDDLE_LEFT]); 
	LoadDivGraph(_T("Resources/Images/mapchip_scaffold_L.bmp"),
		4, 4, 1,
		32, 16,
		&mapchip_graphics_handles[(int)EMapChipType::SC_IRON_L]);
	LoadDivGraph(_T("Resources/Images/mapchip_scaffold_S.bmp"),
		4, 4, 1,
		32, 8,
		&mapchip_graphics_handles[(int)EMapChipType::SC_IRON_S]);

}

void Ground::Update(float delta_seconds)
{
	__super::Update(delta_seconds);

}

void Ground::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	// マップの描画
	for (int j = 0; j < map_data.size(); ++j)
	{
		for (int i = 0; i < map_data[j].size(); ++i)
		{
			std::string c = map_data[j][i];
			DrawGraph(i * MAPCHIP_SIZE - screen_offset.x, j * MAPCHIP_SIZE, mapchip_graphics_handles[std::stoi(c)], false);
		}
	}

}

void Ground::Finalize()
{
	__super::Finalize();
}


