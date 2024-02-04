#include <DxLib.h>
#include "GameOverScene.h"
#include "../../SystemTypes.h"
#include "../../Manager/InputManager/InputManager.h"
#include "../../Manager/SoundManager/SoundManager.h"
#include "../../Manager/GraphicsManager/GraphicsManager.h"


GameOverScene::GameOverScene()
	: SceneBase()
{
	sound_handle = 0;
	graph_handle_gameoverlogo=0;
	graph_handle_gameovertext=0;
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	InputManager::GetInstance().Initialize();

	int size_x = 0, size_y = 0;
	//画像をロード
	graph_handle_gameoverlogo = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/gameoverlogo.png"));
	graph_handle_gameovertext = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/gameovertext.png"));

	//画像を表示させる座標を設定
	GetGraphSize(graph_handle_gameoverlogo, &size_x, &size_y);
	pos_gameoverlogo.x = SCREEN_RESOLUTION_X / 2 - size_x * 0.5f;
	pos_gameoverlogo.y = SCREEN_RESOLUTION_Y / 2 - size_y*0.5;

	GetGraphSize(graph_handle_gameovertext, &size_x, &size_y);
	pos_gameovertext.x = SCREEN_RESOLUTION_X / 2 - size_x * 0.5;
	pos_gameovertext.y = SCREEN_RESOLUTION_Y / 2+size_y *3.0f;

	//SE
	sound_handle = SoundManager::GetInstance().LoadSound("Resources/Sound/gameover.wav");
	PlaySoundMem(sound_handle, DX_PLAYTYPE_BACK, TRUE);
}

ESceneType GameOverScene::Update(float delta_seconds)
{
	if (InputManager::GetInstance().GetKeyDown(KEY_INPUT_C))
		return ESceneType::TITLE_SCENE;

	InputManager::GetInstance().Update();

	return ESceneType::GAMEOVER_SCENE;
}

void GameOverScene::Draw()
{
	DrawBox(0, 0, SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y, GetColor(33,33,33), true);
	DrawGraph(pos_gameoverlogo.x, pos_gameoverlogo.y, graph_handle_gameoverlogo, true);
	DrawGraph(pos_gameovertext.x, pos_gameovertext.y, graph_handle_gameovertext, true);
}

void GameOverScene::Finalize()
{
}