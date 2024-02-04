#include "Stage.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include "../SystemTypes.h"

Stage::Stage(const std::string& file_path)
{
	LoadCSV(file_path);
}


Stage::~Stage()
{
	Finalize();
}

void Stage::Finalize()
{
	for (int i = 0; i < stage_array.size(); ++i)
	{
		stage_array[i].clear();
	}
	stage_array.clear();

	stage_tile_num_width = -1;
	stage_tile_num_height = -1;
	stage_bounds = Vector2D();
}

std::string Stage::GetTile(int x, int y) const
{
	if (x >= 0 && x < stage_tile_num_width && y >= 0 && y < stage_tile_num_height)
	{
		return stage_array[y][x];
	}

	return "";
}

void Stage::SetTile(int x, int y, std::string str)
{
	stage_array[y][x] = str;
}


//戻り値をboolにする
void Stage::LoadCSV(const std::string& path)
{
	std::fstream ifs(path);
	//ファイルが見つからない場合の例外処理
	assert(!ifs.fail());

	std::string line = "";

	int height = 0;

	while (std::getline(ifs, line))
	{
		std::istringstream iis(line);

		stage_array.push_back(std::vector<std::string>());

		std::string conma_buf;
		while (std::getline(iis, conma_buf, ','))
		{
			stage_array[height].push_back(conma_buf);
		}

		height++;
	}
	//マップチップの数をセット
	stage_tile_num_width = stage_array[0].size();
	stage_tile_num_height = stage_array.size();

	stage_bounds = Vector2D(stage_tile_num_width * MAPCHIP_SIZE, stage_tile_num_height * MAPCHIP_SIZE);
}
