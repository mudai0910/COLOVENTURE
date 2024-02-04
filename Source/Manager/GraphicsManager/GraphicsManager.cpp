#include "GraphicsManager.h"
#include "DxLib.h"
#include <stdexcept>

int GraphicsManager::LoadGraph(const std::string& file)
{
	try
	{
		// ロード済みであればそのハンドルを返す.
		int handle = graphics_handles.at(file);
		return handle;
	}
	catch (std::out_of_range&)
	{
		int handle = DxLib::LoadGraph(file.c_str());
		graphics_handles.emplace(file, handle);
		return handle;
	}
}

void GraphicsManager::LoadDivGraph(const std::string& file, int AllNum, int XNum, int YNum, int XSize, int YSize, std::vector<int>& OutHandleArray)
{
	try
	{
		// ロード済みであればそのハンドルを返す.
		int result = graphics_handles.at(file);
		OutHandleArray[0] = result;
		for (int i = 1; i < AllNum; ++i)
			OutHandleArray[i] = graphics_handles.at(file + std::to_string(i));
	}
	catch (std::out_of_range&)
	{
		DxLib::LoadDivGraph(file.c_str(), AllNum, XNum, YNum, XSize, YSize, OutHandleArray.data());
		graphics_handles.emplace(file, OutHandleArray[0]);
		for (int i = 1; i < AllNum; ++i)
			graphics_handles.emplace(file + std::to_string(i), OutHandleArray[i]);
	}
}

void GraphicsManager::AllDeleteGraph()
{
	for (auto pair : graphics_handles)
		DeleteGraph(pair.second);

	graphics_handles.clear();
}
