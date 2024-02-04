#include "SceneManager.h"
#include <cassert>
#include "DxLib.h"
#include "SceneBase.h"
#include "TitleScene/TitleScene.h"
#include "InGameScene/InGameScene.h"
#include "GameClearScene/GameClearScene.h"
#include "GameOverScene/GameOverScene.h"

SceneManager::SceneManager()
	: current_scene(nullptr)
{
}

SceneManager::~SceneManager()
{
	Finalize();
}

void SceneManager::Initialize()
{
	// �J�n�V�[��
	ChangeScene(ESceneType::TITLE_SCENE);
}

void SceneManager::Update(float delta_seconds)
{
	// �V�[���̍X�V
	ESceneType result_scene_type = current_scene->Update(delta_seconds);

	// ���݂�Update����󂯎�����V�[�����ʂ̏ꍇ�A�V�[���̑J�ڂ��s��
	if (result_scene_type != current_scene->GetSceneType())
	{
		ChangeScene(result_scene_type);
	}

	// �`��
	Draw();
}

void SceneManager::Finalize()
{
	// CurrentScene�̉��
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

void SceneManager::Draw()
{
	// ����ʂɕ`��
	SetDrawScreen(DX_SCREEN_BACK);
	// ��ʂ��N���A
	ClearDrawScreen();
	// �V�[���̕`��
	current_scene->Draw();
	// ��ʂ��X�V
	ScreenFlip();
}

void SceneManager::ChangeScene(ESceneType new_scene_type)
{
	// �V�[���̐���
	SceneBase* new_scene = CreateScene(new_scene_type);
	assert(new_scene != nullptr);

	// ���݂̃V�[���̉��
	// �ŏ��̃V�[���������̂�CurrentScene��nullptr
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	// �V�����V�[���̊J�n
	new_scene->Initialize();
	current_scene = new_scene;
}

SceneBase* SceneManager::CreateScene(ESceneType new_scene_type)
{
	switch (new_scene_type)
	{
	case ESceneType::TITLE_SCENE:
		return new TitleScene();
	case ESceneType::INGAME_SCENE:	
		return new InGameScene();
	case ESceneType::GAMEOVER_SCENE:
		return new GameOverScene();
	case ESceneType::GAMECLEAR_SCENE:
		return new GameClearScene();
	default:					
		return nullptr;
	}
}