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
	//�e�I�u�W�F�N�g��Update�����s
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
	// �������Ƀ��[�h�����T�E���h��S�č폜
	SoundManager::GetInstance().AllDeleteSound();
	// �������Ƀ��[�h�����e�N�X�`����S�č폜
	GraphicsManager::GetInstance().AllDeleteGraph();

	// �S�ẴI�u�W�F�N�g��j��
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