#include "TitleScene.h"
#include "../../SystemTypes.h"
#include <DxLib.h>
#include <string>
#include "../../Manager/InputManager/InputManager.h"
#include "../../GameObject/Field/Ground/Ground.h"
#include "../../Manager/GraphicsManager/GraphicsManager.h"
#include "../../Manager/SoundManager/SoundManager.h"
#include "../../Stage/Stage.h"


TitleScene::TitleScene()
	: SceneBase()
{
	stage_data = nullptr;
	sound_handle_button=0;
	graph_handle_background_distant=0;
	graph_handle_background_close=0;
	graph_handle_title =0;
	graph_handle_cursor =0;
	graph_handle_rulewindow =0;
	graph_handle_startbutton_n =0;
	graph_handle_startbutton_s =0;
	graph_handle_rulebutton_n =0;
	graph_handle_rulebutton_s =0;
	button_id = 0;
	is_display_rule=false;
}

TitleScene::~TitleScene()
{
	Finalize();
}

void TitleScene::Initialize()
{
	InputManager::GetInstance().Initialize();

	//�X�e�[�W�f�[�^�̃p�X
	std::string stage_path = "Resources/StageData/Stage_" + std::to_string(0) + ".csv";
	//�X�e�[�W�����[�h
	stage_data = new Stage(stage_path);

	// �n�ʃI�u�W�F�N�g��ǉ�
	Ground* ground = CreateObject<Ground>(Vector2D(0,0));
	//�}�b�v�f�[�^���Z�b�g
	ground->SetMapData(stage_data->GetRawStage());

	//�摜�����[�h
	graph_handle_title = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/title.png"));
	graph_handle_background_distant = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/bg_3_0.png"));
	graph_handle_background_close = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/bg_3_1.bmp"));
	graph_handle_rulewindow = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/rulewindow.png"));
	graph_handle_startbutton_n = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/startbutton_normal.png"));
	graph_handle_startbutton_s = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/startbutton_select.png"));
	graph_handle_rulebutton_n = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/rulebutton_normal.png"));
	graph_handle_rulebutton_s = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/rulebutton_select.png"));
	graph_handle_cursor = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/title_cursor.bmp"));

	button_pos.resize((int)EButtonType::MAX);

	int size_x = 0, size_y = 0;
	//�摜��\����������W��ݒ�
	GetGraphSize(graph_handle_title, &size_x, &size_y);
	pos_title.x = SCREEN_RESOLUTION_X / 2 - size_x * 0.5f;
	pos_title.y = SCREEN_RESOLUTION_Y / 2 - size_y;

	GetGraphSize(graph_handle_rulewindow, &size_x, &size_y);
	pos_rulewindow.x = SCREEN_RESOLUTION_X / 2 - size_x * 0.5f;
	pos_rulewindow.y = SCREEN_RESOLUTION_Y/2 - size_y * 0.5f;

	GetGraphSize(graph_handle_startbutton_n, &size_x, &size_y);
	button_pos[(int)EButtonType::START].x = SCREEN_RESOLUTION_X/2 - size_x * 0.5f;
	button_pos[(int)EButtonType::START].y = SCREEN_RESOLUTION_Y * 0.7f - size_y * 0.5f;

	GetGraphSize(graph_handle_rulebutton_n, &size_x, &size_y);
	button_pos[(int)EButtonType::RULE].x = SCREEN_RESOLUTION_X / 2 - size_x * 0.5f;
	button_pos[(int)EButtonType::RULE].y = SCREEN_RESOLUTION_Y * 0.85f - size_y * 0.5f;

	GetGraphSize(graph_handle_cursor, &size_x, &size_y);
	pos_cursor = button_pos[(int)EButtonType::START];

	sound_handle = SoundManager::GetInstance().LoadSound("Resources/Sound/title.wav");
	PlaySoundMem(sound_handle, DX_PLAYTYPE_LOOP, TRUE);

}

ESceneType TitleScene::Update(float delta_seconds)
{
	InputManager::GetInstance().Update();
	if (InputManager::GetInstance().GetKeyDown(KEY_INPUT_RETURN))
	{
		PlaySoundMem(graph_handle_startbutton_n, DX_PLAYTYPE_NORMAL);
		if ((int)pos_cursor.y == (int)button_pos[(int)EButtonType::START].y&& !(is_display_rule))
		{
			return ESceneType::INGAME_SCENE;
		}
		else if ((int)pos_cursor.y == (int)button_pos[(int)EButtonType::RULE].y)
		{
			//���[���摜�̕\��/��\����؂�ւ�
			is_display_rule = !is_display_rule;
		}
	}
	if (InputManager::GetInstance().GetKeyDown(KEY_INPUT_UP) && !(is_display_rule))
	{
		int a = (button_id - 1);
		int b = (int)button_pos.size();

		button_id = (a % b + b) % b;	// ���̗]��������.
		pos_cursor.y = button_pos[button_id].y;
	}
	if (InputManager::GetInstance().GetKeyDown(KEY_INPUT_DOWN) && !(is_display_rule))
	{
		button_id = (button_id + 1) % (int)button_pos.size();
		pos_cursor.y = button_pos[button_id].y;
	}
	return ESceneType::TITLE_SCENE;
}

void TitleScene::Draw()
{
	//�w�i�̕\��
	DrawExtendGraph(0, 0, SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y,graph_handle_background_distant, true);
	DrawExtendGraph(0, 0, SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y,graph_handle_background_close, true);
	//�e�֐��Ăяo��
	__super::Draw();

	//�^�C�g�����S�̕\��
	DrawGraph(pos_title.x, pos_title.y, graph_handle_title, true);

	DrawGraph(button_pos[(int)EButtonType::START].x, button_pos[(int)EButtonType::START].y, graph_handle_startbutton_n, true);
	DrawGraph(button_pos[(int)EButtonType::RULE].x, button_pos[(int)EButtonType::RULE].y, graph_handle_rulebutton_n, true);
	//�I�����̓I�����W�F�ɂȂ�
	switch (button_id)
	{
	case (int)EButtonType::START:
		DrawGraph(button_pos[(int)EButtonType::START].x, button_pos[(int)EButtonType::START].y, graph_handle_startbutton_s, true);
		break;
	case (int)EButtonType::RULE:
		DrawGraph(button_pos[(int)EButtonType::RULE].x, button_pos[(int)EButtonType::RULE].y, graph_handle_rulebutton_s, true);
		break;
	default:
		break;
	}
	if (is_display_rule)
	{
		//���[���摜��\��
		DrawGraph(pos_rulewindow.x, pos_rulewindow.y, graph_handle_rulewindow, true);
	}
}

void TitleScene::Finalize()
{
	// �e�N���X��Finalize()
	__super::Finalize();

	//new�����I�u�W�F�N�g��j��
	delete stage_data;
	stage_data = nullptr;
}
