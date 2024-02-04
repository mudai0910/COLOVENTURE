#include "SceneBase.h"
#include "../Manager/SoundManager/SoundManager.h"
#include "../Manager/GraphicsManager/GraphicsManager.h"
#include <DxLib.h>

SceneBase::SceneBase()
	: screen_offset(Vector2D())
{
	sound_handle = 0;
}

SceneBase::~SceneBase()
{
	Finalize();
}

void SceneBase::Initialize()
{
}

ESceneType SceneBase::Update(float delta_seconds)
{
	//各オブジェクトのUpdateを実行
	for (auto iterator = objects.begin(); iterator != objects.end();)
	{
		GameObject* object = (*iterator);

		object->Update(delta_seconds);
		++iterator;
	}

	return GetSceneType();
}

void SceneBase::Draw()
{
	for (auto iterator = objects.begin(); iterator != objects.end(); ++iterator)
	{
		(*iterator)->Draw(screen_offset);
	}
}

void SceneBase::Finalize()
{
	StopSoundMem(sound_handle);
	// メモリにロードしたサウンドを全て削除
	SoundManager::GetInstance().AllDeleteSound();
	// メモリにロードしたテクスチャを全て削除
	GraphicsManager::GetInstance().AllDeleteGraph();

	// 全てのオブジェクトを破棄
	DestroyAllObjects();
}


void SceneBase::DestroyAllObjects()
{
	for (auto iterator = objects.begin(); iterator != objects.end(); ++iterator)
	{
		(*iterator)->Finalize();
		delete (*iterator);
	}
	objects.clear();
}