#pragma once

#include <vector>
#include <map>
#include <string>

class GraphicsManager
{
private:
	GraphicsManager() = default;
	~GraphicsManager() = default;

public:
	GraphicsManager(const GraphicsManager&) = delete;
	GraphicsManager& operator=(const GraphicsManager&) = delete;
	GraphicsManager(GraphicsManager&&) = delete;
	GraphicsManager& operator=(GraphicsManager&&) = delete;

	static GraphicsManager& GetInstance() {
		static GraphicsManager instance;
		return instance;
	}

	int LoadGraph(const std::string& file);
	void LoadDivGraph(const std::string& file, int AllNum, int XNum, int YNum, int XSize, int YSize, std::vector<int>& OutHandleArray);
	void AllDeleteGraph();

private:

	std::map<std::string, int> graphics_handles;
};

