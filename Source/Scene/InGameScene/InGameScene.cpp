#include <DxLib.h>
#include <string>
#include <fstream>
#include <cassert>
#include "InGameScene.h"
#include "../../SystemTypes.h"
#include "../../GameObject/Character/Player/Player.h"
#include "../../GameObject/Character/Enemy/Murasuke/Murasuke.h"
#include "../../GameObject/Character/Enemy/Akasuke/Akasuke.h"
#include "../../GameObject/Item/Coin/Coin.h"
#include "../../GameObject/Item/EnergyDrink/EnergyDrink.h"
#include "../../GameObject/Gimmick/GoalFlag/GoalFlag.h"
#include "../../GameObject/Field/Ground/Ground.h"
#include "../../GameObject/Field/BackGround/BackGround.h"
#include "../../Stage/Stage.h"
#include "../../ObjectTypes.h"
#include "../../Manager/GraphicsManager/GraphicsManager.h"
#include "../../Manager/SoundManager/SoundManager.h"

InGameScene::InGameScene()
{
	stage_data = nullptr;
	player = nullptr;
	ui_handle = 0;
	current_stage = 1;
	BuildStageData(current_stage);
	time = 100;
	total_coin = 0;
	total_score = 0;
	player_left = 2;
	stage_waiting_time=0;
	sound_handle_stageclear = 0;
	sound_handle_miss = 0;
	sound_handle_1up = 0;
}
InGameScene::~InGameScene()
{
	Finalize();
}
void InGameScene::Initialize()
{
	// �e�N���X��Initialize()
	__super::Initialize();

	player_respawn_point = Vector2D();
	//�X�|�[���ʒu�̏�����
	object_spawn_points.clear();
	//�w�i��BGM�����[�h����
	LoadStage(current_stage);

	// �n�ʃI�u�W�F�N�g��ǉ�
	Ground* ground = CreateObject<Ground>(Vector2D(0.f, 0.f));
	//���X�|�[������K�v�̂���Q�[���I�u�W�F�N�g�̃C���f�b�N�X�̎n�܂���擾
	respawn_object_index = objects.size();

	//�v���C���[�̊J�n�n�_
	Vector2D player_spawn_point;

	// �^�C���}�b�v�Ɋ�Â����Q�[���I�u�W�F�N�g�̃X�|�[��
	for (int y = 0; y < stage_data->GetStageTileNumY(); ++y)
	{
		for (int x = 0; x < stage_data->GetStageTileNumX(); ++x)
		{
			if (stage_data->GetTile(x, y) == "P")
			{
				//�I�u�W�F�N�g�̃X�|�[���ʒu��ێ�
				player_spawn_point = Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE);
				// ���̃^�C���͋�̃^�C���Ƃ��ēo�^���Ȃ���
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "C")
			{
				object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<Coin>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "E")
			{
				object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<EnergyDrink>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "M")
			{
				object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<Murasuke>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "A")
			{
				object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<Akasuke>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "G")
			{
			   object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<GoalFlag>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
		}
	}

	object_spawn_points.push_back(player_spawn_point);
	//�v���C���[���X�|�[�����`�悪��ԏ�ɗ���悤�ɍŌ�ɃX�|�[������
	player = CreateObject<Player>(object_spawn_points.back());

	//�}�b�v�f�[�^���Z�b�g
	ground->SetMapData(stage_data->GetRawStage());
	//�R�C���ƃX�R�A���Z�b�g
	//player->SetCoinNum(total_coin);
	player->SetScore(total_score);
	ui_handle = CreateFontToHandle(NULL, 24, 2);
	elapsed_time = 0.0f;
	time = 100;

	//�摜�����[�h����
	graph_handle_hpbar_base = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/hpbar_base.bmp"));
	graph_handle_hpbar_meter = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/hpbar_meter.bmp"));
	graph_handle_hpbar_flame = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/hpbar_flame.bmp"));
	graph_handle_icon_coin = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/mapchip_035.bmp"));
	graph_handle_icon_colon = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/collon_icon.bmp"));

	//SE�����[�h����
	sound_handle_stageclear = SoundManager::GetInstance().LoadSound("Resources/Sound/stageclear.wav");
	sound_handle_miss = SoundManager::GetInstance().LoadSound("Resources/Sound/miss.wav");
	sound_handle_1up = SoundManager::GetInstance().LoadSound("Resources/Sound/1up.wav");

	gameplay_state = EGamePlayState::STAGESTART;
}

ESceneType InGameScene::Update(float delta_seconds)
{
	//�v���C��Ԃɉ����ĕ���
	switch (gameplay_state)
	{
	case EGamePlayState::PLAYING:
		// �e�N���X��Update()
		__super::Update(delta_seconds);

		// �I�u�W�F�N�g���m�̏d�Ȃ�𔻒�.
		CheckObjectCollided();

		//�v���C���[���X�e�[�W�O�ɍs���Ȃ��悤�ɂ���(�}�b�v�����Q�Ƃ��邽�߂��̃N���X�ɋL�q)
		if (player->GetPosition().x < 0.0f)
		{
			player->SetPosition(Vector2D(0.0f, player->GetPosition().y));
		}
		else if (player->GetPosition().x + player->GetBoxCollision().x > stage_data->GetStageTileNumX() * MAPCHIP_SIZE)
		{
			player->SetPosition(Vector2D(stage_data->GetStageTileNumX() * MAPCHIP_SIZE - player->GetBoxCollision().x, player->GetPosition().y));

		}
		// �J�������W�̍X�V
		UpdateCameraOffset();
		// �c�莞�Ԃ̍X�V
		elapsed_time += delta_seconds;
		if (elapsed_time >= 1.0f)
		{
			time = (time > 0) ? time - 1 : 0;
			elapsed_time = 0.0f;
		}
		//�R�C��100����1UP
		if (player->GetCoinNum() !=0&&(total_coin + player->GetCoinNum())% 100 == 0)
		{
			//SE
			PlaySoundMem(sound_handle_1up, DX_PLAYTYPE_BACK, TRUE);
			//1UP
			player_left++;
			//�g�[�^���R�C���̖�����ێ�
			total_coin += player->GetCoinNum();
			//����������
			player->SetCoinNum(0);
		}
		//�v���C���[������OR���Ԑ؂�
		if (player->is_player_died || time <= 0)
		{
			StopSoundMem(sound_handle);
			//�~�XSE
			PlaySoundMem(sound_handle_miss, DX_PLAYTYPE_BACK, TRUE);
			//MISS�ɑJ��
			gameplay_state = EGamePlayState::MISS;
			return GetSceneType();
		}
		//�v���C���[���S�[��
		if (player->is_player_goal)
		{
			StopSoundMem(sound_handle);
			//�X�e�[�W�N���ASE
			PlaySoundMem(sound_handle_stageclear, DX_PLAYTYPE_BACK, TRUE);
			//STAGECLEAR�ɑJ��
			gameplay_state = EGamePlayState::STAGECLEAR;
			return GetSceneType();
		}

		return GetSceneType();
		break;

	case EGamePlayState::STAGECLEAR:
		//�X�e�[�W�N���ASE�̍Đ����I�������
		if (CheckSoundMem(sound_handle_stageclear) == 0)
		{
			//���̃��x���������
			if (current_stage + 1 < (int)EStageID::MAX)
			{
				//�R�C���ƃX�R�A��ێ�
				total_coin += player->GetCoinNum();
				//�R�C���̖����̏�����
				player->SetCoinNum(0);
				total_score = player->GetScore();
				//���݂̃X�e�[�WID���X�V
				++current_stage;
				//���̃��x����
				BuildStageData(current_stage);
				gameplay_state = EGamePlayState::STAGESTART;
				return GetSceneType();
			}
			else  //�Q�[���N���A�V�[���ɑJ��
			{
				return ESceneType::GAMECLEAR_SCENE;
			}
		}
		return GetSceneType();
		break;

	case EGamePlayState::MISS:
		//�~�XSE�̍Đ����I�������
		if (CheckSoundMem(sound_handle_miss) == 0)
		{
			//�c�@�����炷
			player_left--;
			if (player_left >= 0)
			{
				//�n�ʂ������e�I�u�W�F�N�g�̃��X�|�[������
				for (int i = 0; i < object_spawn_points.size(); ++i)
				{
					objects[respawn_object_index + i]->Respawn(object_spawn_points[i]);
				}
				// �c�莞�Ԃ����ɖ߂�
				time = 100;
				gameplay_state = EGamePlayState::STAGESTART;
				return GetSceneType();
			}
			else
			{
				//�c��0�ŃQ�[���I�[�o�[
				return ESceneType::GAMEOVER_SCENE;
			}
		}
		return GetSceneType();
		break;

	case EGamePlayState::STAGESTART:
		//�����o�߂�����X�e�[�W�J��
		stage_waiting_time += delta_seconds;
		if (stage_waiting_time < 1.5f)
		{
			return GetSceneType();
		}
		else
		{
			stage_waiting_time = 0.0f;
			//BGM�Đ�
			PlaySoundMem(sound_handle, DX_PLAYTYPE_LOOP, TRUE);
			//�v���C��ʂ�
			gameplay_state = EGamePlayState::PLAYING;
			return GetSceneType();
		}
		break;
	}

}

void InGameScene::Draw()
{

	// �e�N���X��Draw()
	__super::Draw();

	//UI
	//�\������R�C���̖���
	int current_coin_num = player->GetCoinNum() + total_coin;
	std::string coin_text = std::to_string(current_coin_num);
	std::string score_text = "SCORE " + std::to_string(player->GetScore());
	std::string time_text = "TIME " + std::to_string(time);
	std::string stage_text = "STAGE " + std::to_string((int)current_stage);

	if (gameplay_state == EGamePlayState::STAGESTART)
	{
		DrawBox(0, 0, SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y, 0, true);
		//�X�e�[�W���𒆉��ɕ\��
		int text_width = GetDrawStringWidthToHandle(stage_text.c_str(), stage_text.size(), ui_handle);
		DrawStringToHandle(SCREEN_RESOLUTION_X/2 - text_width * 0.5f, SCREEN_RESOLUTION_Y/2-50, stage_text.c_str(), GetColor(255, 255, 255), ui_handle);
		//�c�@�̕\��
		std::string left_text = "�~" + std::to_string(player_left);
		text_width = GetDrawStringWidthToHandle(left_text.c_str(), left_text.size(), ui_handle);
		DrawGraph(SCREEN_RESOLUTION_X / 2 - text_width*1.5f, SCREEN_RESOLUTION_Y / 2, graph_handle_icon_colon, true);
		DrawStringToHandle(SCREEN_RESOLUTION_X / 2 + text_width * 0.5f, SCREEN_RESOLUTION_Y / 2+5, left_text.c_str(), GetColor(255, 255, 255), ui_handle);
	}
	//�㕔��UI
	const int FONT_HEIGHT = 32;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, SCREEN_RESOLUTION_X, FONT_HEIGHT * 1.5f, 0, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);

	DrawGraph(SCREEN_RESOLUTION_X * 0.05f, 5, graph_handle_hpbar_base, true);
	Vector2D healthbar_tl = Vector2D(SCREEN_RESOLUTION_X * 0.05f, 5);
	Vector2D healthbar_br = healthbar_tl + Vector2D(160 * player->GetHPRatio(), FONT_HEIGHT);
	DrawExtendGraphF(healthbar_tl.x, healthbar_tl.y, healthbar_br.x, healthbar_br.y, graph_handle_hpbar_meter, true);
	DrawGraph(SCREEN_RESOLUTION_X * 0.05f, 5, graph_handle_hpbar_flame, true);


	DrawGraph(SCREEN_RESOLUTION_X * 0.32f, 5, graph_handle_icon_coin, true);
	DrawStringToHandle(SCREEN_RESOLUTION_X * 0.4f, 10, coin_text.c_str(), GetColor(255, 255, 255), ui_handle);
	DrawStringToHandle(SCREEN_RESOLUTION_X * 0.55f,10, score_text.c_str(), GetColor(255, 255, 255), ui_handle);
	DrawStringToHandle(SCREEN_RESOLUTION_X * 0.8f, 10, time_text.c_str(), GetColor(255, 255, 255), ui_handle);
}

void InGameScene::BuildStageData(int stage_id)
{
	bool need_initialize = false;
	// ���łɑ��̃��x���f�[�^��ǂݍ��ݍς݂ł����.
	if (stage_data)
	{
		stage_data->Finalize();
		//new�����I�u�W�F�N�g��j��
		delete stage_data;
		stage_data = nullptr;

		need_initialize = true;
	}
	//�X�e�[�W�f�[�^�̃p�X
	std::string stage_path = "Resources/StageData/Stage_" + std::to_string(stage_id) + ".csv";
	//�X�e�[�W�����[�h
	stage_data = new Stage(stage_path);
	//�X�e�[�W�X�V���ɏ�����
	if (need_initialize)
	{
		//�A�Z�b�g��
		Finalize();
		Initialize();
	}
}
//�X�e�[�W���L�̃f�[�^�����[�h����
void InGameScene::LoadStage(int stage_id)
{
	BackGround* background0 = CreateObject<BackGround>(Vector2D(0.f, 0.f));
	BackGround* background1 = CreateObject<BackGround>(Vector2D(0.f, 0.f));

	switch ((EStageID)stage_id)
	{
	case EStageID::STAGE_1:
		sound_handle = SoundManager::GetInstance().LoadSound("Resources/Sound/oceanstage.wav");
		background0->LoadGraphics("Resources/Images/bg_1_0.png");
		background0->SetDistanceScale(100.f);
		background1->LoadGraphics("Resources/Images/bg_1_1.png");
		background1->SetDistanceScale(5.f);
		break;
	case EStageID::STAGE_2:
		sound_handle = SoundManager::GetInstance().LoadSound("Resources/Sound/foreststage.wav");
		background0->LoadGraphics("Resources/Images/bg_2_0.png");
		background0->SetDistanceScale(100.f);
		background1->LoadGraphics("Resources/Images/bg_2_1.png");
		background1->SetDistanceScale(5.f);
		break;
	default:
		break;
	}
}
//�����蔻��̃`�F�b�N
void InGameScene::CheckObjectCollided()
{
	// ��`���m�̏Փ˔���.
	for (int i = 0; i < objects.size(); ++i)
	{
		GameObject* obj1 = objects[i];

		//Character�N���X�͒n�ʂƂ̏Փ˔��������
		if (obj1->GetObjectType() == EObjectType::PLAYER || obj1->GetObjectType() == EObjectType::ENEMY)
		{
			Character* character = dynamic_cast<Character*>(obj1);
			//�n�ʂƂ̏Փ˔���
			HandleCollisionToTerrain(character);
		}
		//��ʊO�ɂ���I�u�W�F�N�g�͓����蔻��ΏۊO
		if (obj1->GetPosition().x < screen_offset.x || screen_offset.x + SCREEN_RESOLUTION_X < (obj1->GetPosition().x))continue;

		Vector2D obj1_center = obj1->GetPosition() + obj1->GetBoxCollision() * 0.5f;
		for (int j = i + 1; j < objects.size(); ++j)
		{
			GameObject* obj2 = objects[j];
			Vector2D obj2_center = obj2->GetPosition() + obj2->GetBoxCollision() * 0.5f;
			//��ʊO�ɂ���I�u�W�F�N�g�͓����蔻��ΏۊO
			if (obj2->GetPosition().x < screen_offset.x || screen_offset.x + SCREEN_RESOLUTION_X < (obj2->GetPosition().x))continue;

			// �Փ˔���
			if (OverlapObjects(obj1, obj2))
			{
				//�o���ɒʒm
				obj1->OnCollision(obj2);
				obj2->OnCollision(obj1);
			}
		}
	}
}
//�I�u�W�F�N�g���m�̏d�Ȃ�𔻒�
bool InGameScene::OverlapObjects(const GameObject* A, const GameObject* B)
{
	float A_top = A->GetPosition().y;
	float A_bottom = A->GetPosition().y + A->GetBoxCollision().y;
	float A_left = A->GetPosition().x;
	float A_right = A->GetPosition().x + A->GetBoxCollision().x;

	float B_top = B->GetPosition().y;
	float B_bottom = B->GetPosition().y + B->GetBoxCollision().y;
	float B_left = B->GetPosition().x;
	float B_right = B->GetPosition().x + B->GetBoxCollision().x;

	if (A_top > B_bottom		// A�̏�ʂ�B�̒�ʂ����Ⴂ
		|| A_right < B_left		// A�̉E�ʂ�B�̍��ʂ������ɂ���
		|| A_bottom < B_top		// A�̒�ʂ�B�̏�ʂ�������
		|| A_left > B_right)	// A�̍��ʂ�B�̉E�ʂ����E�ɂ���
		return false;

	return true;
}
//�n�ʂƃL�����N�^�[�̃R���W��������
void InGameScene::HandleCollisionToTerrain(Character* character)
{
	assert(character != nullptr);

	std::string tile;

	int tile_pos_left_x = std::floor(character->GetPosition().x / MAPCHIP_SIZE);  //������x�ʒu
	int tile_pos_right_x = std::floor((character->GetPosition().x + character->GetBoxCollision().x) / MAPCHIP_SIZE);  //�E����x�ʒu
	int tile_pos_upper_y = std::floor(character->GetPosition().y / MAPCHIP_SIZE);  //�㑤��y�ʒu
	int tile_pos_lower_y = std::floor((character->GetPosition().y + character->GetBoxCollision().y) / MAPCHIP_SIZE);  //������y�ʒu

	// �A�N�^�[�̏�ʂ̏Փ˂��e�X�g
	{
		tile = stage_data->GetTile(tile_pos_left_x, tile_pos_upper_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_left_x, tile_pos_upper_y);
		tile = stage_data->GetTile(tile_pos_right_x, tile_pos_upper_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_right_x, tile_pos_upper_y);
	}

	// �A�N�^�[�̒�ʂ̏Փ˂��e�X�g
	{
		//�v���C���[�͗��������ꂽ�痎�����A�G�͕Б������ꂽ������]������
		if(stage_data->GetTile(tile_pos_left_x, tile_pos_lower_y)=="0" && stage_data->GetTile(tile_pos_right_x, tile_pos_lower_y)=="0")
		{
			character->OnFallAllFoots();
		}
		else if (stage_data->GetTile(tile_pos_left_x, tile_pos_lower_y) == "0" || stage_data->GetTile(tile_pos_right_x, tile_pos_lower_y) == "0")
		{
			character->OnReachDeadEnd();
		}
		else
		{
			ResolveCollisionToTerrain(character, tile_pos_left_x, tile_pos_lower_y);
			ResolveCollisionToTerrain(character, tile_pos_right_x, tile_pos_lower_y);
		}
	}

	// �A�N�^�[�̍��ʂ̏Փ˂��e�X�g
	{
		tile = stage_data->GetTile(tile_pos_left_x, tile_pos_upper_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_left_x, tile_pos_upper_y);
		tile = stage_data->GetTile(tile_pos_left_x, tile_pos_lower_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_left_x, tile_pos_lower_y);
	}

	// �A�N�^�[�̉E�ʂ̏Փ˂��e�X�g
	{
		tile = stage_data->GetTile(tile_pos_right_x, tile_pos_upper_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_right_x, tile_pos_upper_y);
		tile = stage_data->GetTile(tile_pos_right_x, tile_pos_lower_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_right_x, tile_pos_lower_y);
	}
}

//�n�ʂ̃R���W��������
void InGameScene::ResolveCollisionToTerrain(Character* character, int tile_pos_x, int tile_pos_y)
{
	std::string str = stage_data->GetTile(tile_pos_x, tile_pos_y);
	if (str.empty())
	{
		return;
	}
	int tile_type = std::stoi(str);
	// ���x���f�[�^�ɓo�^����Ă���ԍ��ɉ����ďՓ˔��肷��ӂ�ύX����
	// tile_type��10~16�͑���B�������̈���ʍs�ɂ���
	if (tile_type >= 10 && tile_type <= 16)
	{
		// �L�����N�^�[�̒�ʂƑ���̏�ʂ݂̂̏Փ˔���
		CollideTop(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 1)
	{
		if (CollideLeft(character, tile_pos_x, tile_pos_y)) return;
		CollideTop(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 2)
	{
		CollideTop(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 3)
	{
		if (CollideRight(character, tile_pos_x, tile_pos_y)) return;
		CollideTop(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 4)
	{
		CollideLeft(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 5)
	{
		if (CollideTop(character, tile_pos_x, tile_pos_y)) return;
		if (CollideLeft(character, tile_pos_x, tile_pos_y)) return;
		if (CollideRight(character, tile_pos_x, tile_pos_y)) return;
		CollideBottom(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 6)
	{
		CollideRight(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 7)
	{
		if (CollideLeft(character, tile_pos_x, tile_pos_y)) return;
		CollideBottom(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 8)
	{
		CollideBottom(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 9)
	{
		if (CollideRight(character, tile_pos_x, tile_pos_y)) return;
		CollideBottom(character, tile_pos_x, tile_pos_y);
		return;
	}

	return;
}
bool InGameScene::CollideTop(Character* character, int tile_pos_x, int tile_pos_y)
{
	float top = tile_pos_y * MAPCHIP_SIZE;

	// �u�Փ˂����u�ԁv�ł��邱�Ƃ�m�肽��.
	// �O�t���[���̈ʒu���Փ˔͈͊O�ł���A���t���[�����Փ˔͈͓��ł���΁u�Փ˂����u�ԁv���Ƃ킩��.
	// �A�N�^�[��ʂ̓^�C���̏�ʂ����Ⴂ�ʒu�ɂ���
	// && �O�t���[���̃A�N�^�[��ʂ̓^�C���̏�ʂ��������ʒu�ɂ���
	if (character->GetPosition().y + character->GetBoxCollision().y > top
		&& character->GetPrevPosition().y + character->GetBoxCollision().y <= top)
	{
		Vector2D vel = character->GetVelocity();
		character->SetVelocity(Vector2D(vel.x, 0.0f));
		Vector2D pos = character->GetPosition();
		character->SetPosition(Vector2D(pos.x, top - character->GetBoxCollision().y ));
		Vector2D prev_pos = character->GetPrevPosition();
		character->SetPrevPosition(Vector2D(prev_pos.x, pos.y));
		// ���炩�̒n�ʁE����ɗ����Ă���̂Łu�������[�h�v�Ɉڍs.
		character->ShiftMovementState(EMovementState::WALKING);
		return true;
	}

	return false;
}

bool InGameScene::CollideBottom(Character* character, int tile_pos_x, int tile_pos_y)
{
	float bottom = tile_pos_y * MAPCHIP_SIZE + MAPCHIP_SIZE;

	// �A�N�^�[��ʂ̓^�C���̒�ʂ��������ʒu�ɂ���
	// && �O�t���[���̃A�N�^�[��ʂ̓^�C���̒�ʂ����Ⴂ�ʒu�ɂ���
	if (character->GetPosition().y < bottom
		&& character->GetPrevPosition().y >= bottom)
	{
		Vector2D vel = character->GetVelocity();
		character->SetVelocity(Vector2D(vel.x, 0.0f));
		Vector2D pos = character->GetPosition();
		character->SetPosition(Vector2D(pos.x, bottom));
		Vector2D prev_pos = character->GetPrevPosition();
		character->SetPrevPosition(Vector2D(prev_pos.x, pos.y));

		return true;
	}
	return false;
}

bool InGameScene::CollideLeft(Character* character, int tile_pos_x, int tile_pos_y)
{
	float left = tile_pos_x * MAPCHIP_SIZE;

	// �A�N�^�[�̉E�ʂ̓^�C�����ʂ����E�̈ʒu�ɂ���
	// && �O�t���[���̃A�N�^�[�̉E�ʂ̓^�C�����ʂ������̈ʒu�ɂ���
	if (character->GetPosition().x + character->GetBoxCollision().x > left
		&& character->GetPrevPosition().x + character->GetBoxCollision().x <= left)
	{
		Vector2D vel = character->GetVelocity();
		character->SetVelocity(Vector2D(0.0f, vel.y));
		Vector2D pos = character->GetPosition();
		character->SetPosition(Vector2D(left - character->GetBoxCollision().x /*- 0.01f*/, pos.y));
		Vector2D prev_pos = character->GetPrevPosition();
		character->SetPrevPosition(Vector2D(pos.x, prev_pos.y));
		//�ǂɂԂ��������Ƃ�ʒm
		character->OnReachDeadEnd();
		return true;
	}

	return false;
}

bool InGameScene::CollideRight(Character* character, int tile_pos_x, int tile_pos_y)
{
	float right = tile_pos_x * MAPCHIP_SIZE + MAPCHIP_SIZE;

	// �A�N�^�[���ʂ̓^�C���E�ʂ������̈ʒu�ɂ���
	// && �O�t���[���̃A�N�^�[���ʂ̓^�C���E�ʂ����E�̈ʒu�ɂ���
	if (character->GetPosition().x < right
		&& character->GetPrevPosition().x >= right)
	{
		Vector2D vel = character->GetVelocity();
		character->SetVelocity(Vector2D(0.0f, vel.y));
		Vector2D pos = character->GetPosition();
		character->SetPosition(Vector2D(right, pos.y));
		Vector2D prev_pos = character->GetPrevPosition();
		character->SetPrevPosition(Vector2D(pos.x, prev_pos.y));
		//�ǂɂԂ��������Ƃ�ʒm
		character->OnReachDeadEnd();
		return true;
	}

	return false;
}
//�J�����̃X�N���[��
void InGameScene::UpdateCameraOffset()
{
	// �X�N���[������40���𒴂��ĉE�Ɉړ�����ƃJ�������ړ�����.
	//
	const float SCROLL_BORDER_RIGHT = SCREEN_RESOLUTION_X * 0.4f;
	const float SCROLL_BORDER_LEFT = SCREEN_RESOLUTION_X * 0.2f;
	//�ő�ŃX�N���[���ł���E�[�̈ʒu
	const float MAX_SCROLL_RIGHT = stage_data->GetStageTileNumX() * MAPCHIP_SIZE - SCREEN_RESOLUTION_X;
	//�v���C���[�̃X�N���[����̈ʒu
	float diff = player->GetPosition().x - screen_offset.x;
	if (diff > SCROLL_BORDER_RIGHT)
	{
		screen_offset.x += diff - SCROLL_BORDER_RIGHT;
	}
	else if (diff < SCROLL_BORDER_LEFT)
	{
		screen_offset.x += diff - SCROLL_BORDER_LEFT;
	}

	if (screen_offset.x > MAX_SCROLL_RIGHT)
	{
		screen_offset.x = MAX_SCROLL_RIGHT;
	}
	else if (screen_offset.x < 0)
	{
		screen_offset.x = 0.0f;
	}
}

void InGameScene::Finalize()
{
	// �e�N���X��Finalize()
	__super::Finalize();
}