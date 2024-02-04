#include "DxLib.h"

#include "SystemTypes.h"
#include "Scene/SceneManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ��ʉ𑜓x�̕ύX
	SetGraphMode(SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y, SCREEN_COLOR_BIT_NUM);
	// Window���[�h�ɕύX
	ChangeWindowMode(true);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// ���ߐF��ݒ�
	SetTransColor(TRANS_COLOR_R, TRANS_COLOR_G, TRANS_COLOR_B);

	// SceneManager�̐���
	SceneManager* scene_manager = new SceneManager();
	scene_manager->Initialize();

	// ���C�����[�v
	int prev_frame_time = GetNowCount();
	while (ProcessMessage() != -1 && CheckHitKey(KEY_INPUT_ESCAPE) != 1)
	{
		// DeltaSeconds�̌v�Z(�b)
		int current_frame_time = GetNowCount();
		float delta_seconds = (float)(current_frame_time - prev_frame_time) / 1000.0f;
		prev_frame_time = current_frame_time;

		// SceneManager�̍X�V
		scene_manager->Update(delta_seconds);

	}

	// SceneManager�̔j��
	scene_manager->Finalize();
	delete scene_manager;
	scene_manager = nullptr;

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;
}