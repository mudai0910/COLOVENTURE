#pragma once

#include <string>
#include <vector>
#include "../Utility/Vector2D.h"

class Stage
{
public:
	Stage(const std::string& file_path);
	~Stage();

	void Finalize();

	const std::vector<std::vector<std::string>>& GetRawStage() const { return stage_array; }
	const int GetStageTileNumX() const { return stage_tile_num_width; }
	const int GetStageTileNumY() const { return stage_tile_num_height; }
	const Vector2D& GetStageBounds() const { return stage_bounds; }
	std::string GetTile(int x, int y) const;
	void SetTile(int x, int y, std::string str);

private:
	void LoadCSV(const std::string& path);

private:
	std::vector<std::vector<std::string>> stage_array;
	int stage_tile_num_width;
	int stage_tile_num_height;
	Vector2D stage_bounds;


};

