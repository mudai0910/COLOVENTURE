#include "GameClearScene.h"
#include "../../SystemTypes.h"
#include <DxLib.h>
#include "../../Manager/InputManager/InputManager.h"
#include "../../Manager/SoundManager/SoundManager.h"
#include "../../Manager/GraphicsManager/GraphicsManager.h"

GameClearScene::GameClearScene()
	: SceneBase()
{
	sound_handle = 0;
	graph_handle_background = 0;
	graph_handle_gameclearlogo = 0;
	graph_handle_gamecleartext = 0;
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{
	InputManager::GetInstance().Initialize();

	int size_x = 0, size_y = 0;
	//画像をロード
	graph_handle_background = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/bg_gameclear.png"));
	graph_handle_gameclearlogo = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/gameclearlogo.png"));
	graph_handle_gamecleartext = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/gamecleartext.png"));

	//画像を表示させる座標を設定
	GetGraphSize(graph_handle_gameclearlogo, &size_x, &size_y);
	pos_gameoverlogo.x = SCREEN_RESOLUTION_X / 2 - size_x * 0.5f;
	pos_gameoverlogo.y = SCREEN_RESOLUTION_Y / 2 - size_y * 0.5;

	GetGraphSize(graph_handle_gamecleartext, &size_x, &size_y);
	pos_gameovertext.x = SCREEN_RESOLUTION_X / 2 - size_x * 0.5;
	pos_gameovertext.y = SCREEN_RESOLUTION_Y / 2 + size_y * 3.0f;
	sound_handle = SoundManager::GetInstance().LoadSound("Resources/Sound/gameclear.wav");
	PlaySoundMem(sound_handle, DX_PLAYTYPE_BACK, TRUE);
}

ESceneType GameClearScene::Update(float delta_seconds)
{
	if (InputManager::GetInstance().GetKeyDown(KEY_INPUT_C))
		return ESceneType::TITLE_SCENE;

	InputManager::GetInstance().Update();

	return ESceneType::GAMECLEAR_SCENE;

}

void GameClearScene::Draw()
{
	DrawGraph(0,0, graph_handle_background, true);
	DrawGraph(pos_gameoverlogo.x, pos_gameoverlogo.y, graph_handle_gameclearlogo, true);
	DrawGraph(pos_gameovertext.x, pos_gameovertext.y, graph_handle_gamecleartext, true);
}

void GameClearScene::Finalize()
{
	__super::Finalize();
}