#pragma once

#include <vector>
#include <map>
#include <string>

class SoundManager
{
private:
	SoundManager() = default;
	~SoundManager() = default;

public:
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	int LoadSound(const std::string& file);
	void LoadDivSound(const std::string& file, int AllNum, int XNum, int YNum, int XSize, int YSize, std::vector<int>& OutHandleArray);
	void AllDeleteSound();

private:

	std::map<std::string, int> sound_handles;
};

