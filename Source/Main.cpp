#include "DxLib.h"

#include "SystemTypes.h"
#include "Scene/SceneManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 画面解像度の変更
	SetGraphMode(SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y, SCREEN_COLOR_BIT_NUM);
	// Windowモードに変更
	ChangeWindowMode(true);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 透過色を設定
	SetTransColor(TRANS_COLOR_R, TRANS_COLOR_G, TRANS_COLOR_B);

	// SceneManagerの生成
	SceneManager* scene_manager = new SceneManager();
	scene_manager->Initialize();

	// メインループ
	int prev_frame_time = GetNowCount();
	while (ProcessMessage() != -1 && CheckHitKey(KEY_INPUT_ESCAPE) != 1)
	{
		// DeltaSecondsの計算(秒)
		int current_frame_time = GetNowCount();
		float delta_seconds = (float)(current_frame_time - prev_frame_time) / 1000.0f;
		prev_frame_time = current_frame_time;

		// SceneManagerの更新
		scene_manager->Update(delta_seconds);

	}

	// SceneManagerの破棄
	scene_manager->Finalize();
	delete scene_manager;
	scene_manager = nullptr;

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;
}