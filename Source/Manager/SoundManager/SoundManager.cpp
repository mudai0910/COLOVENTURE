#include "SoundManager.h"
#include "DxLib.h"
#include <stdexcept>

int SoundManager::LoadSound(const std::string& file)
{
	try
	{
		// ロード済みであればそのハンドルを返す.
		int handle = sound_handles.at(file);
		return handle;
	}
	catch (std::out_of_range&)
	{
		int handle = DxLib::LoadSoundMem(file.c_str());
		sound_handles.emplace(file, handle);
		return handle;
	}
}

void SoundManager::AllDeleteSound()
{
	for (auto pair : sound_handles)
		DeleteSoundMem(pair.second);

	sound_handles.clear();
}
