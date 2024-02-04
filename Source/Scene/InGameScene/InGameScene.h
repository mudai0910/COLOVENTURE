#pragma once
#include <string>
#include "../SceneBase.h"

class Player;
class Character;
class Stage;

class InGameScene : public SceneBase
{

	enum class EStageID : unsigned int
	{
		TITLE_STAGE,
		STAGE_1,
		STAGE_2,

		MAX
	};
	// �Q�[���v���C���ɑJ�ڂ����Ԃ�\��
	enum class EGamePlayState : unsigned char
	{
		PLAYING,	// �v���C��
		STAGECLEAR,	// �N���A���o
		MISS,	    // �~�X���o
		STAGESTART,      // �X�^�[�g
	};

public:
	InGameScene();
	virtual	~InGameScene();

	virtual void Initialize() override;
	virtual ESceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	virtual ESceneType GetSceneType() const override { return ESceneType::INGAME_SCENE; }

private:
	//�X�e�[�W�����[�h
	void BuildStageData(int stage_id);
	// �I�u�G�W�F�N�g���m�̏Փ˂��`�F�b�N.
	void CheckObjectCollided();
	// �����̃I�u�W�F�N�g���d�Ȃ��Ă��邩����
	bool OverlapObjects(const GameObject* obj1, const GameObject* obj2);
	// �L�����N�^�[�ƒn�`�Ƃ̏Փ˂𔻒�.
	void HandleCollisionToTerrain(class Character* character);
	// �L�����N�^�[�ƒn�`�̂߂荞�݂���������.
	void ResolveCollisionToTerrain(class Character* character, int tile_pos_x, int tile_pos_y);

	// �^�C����ʂƂ̏Փ�
	bool CollideTop(class Character* character, int tile_pos_x, int tile_pos_y);	//bool CollideTop(class GameObject* character, int tile_pos_x, int tile_pos_y);
	// �^�C����ʂƂ̏Փ�
	bool CollideBottom(class Character* character, int tile_pos_x, int tile_pos_y);	//bool CollideBottom(class GameObject* character, int tile_pos_x, int tile_pos_y);
	// �^�C�����ʂƂ̏Փ�
	bool CollideLeft(class Character* character, int tile_pos_x, int tile_pos_y);	//bool CollideLeft(class GameObject* character, int tile_pos_x, int tile_pos_y);
	// �^�C���E�ʂƂ̏Փ�
	bool CollideRight(class Character* character, int tile_pos_x, int tile_pos_y);	//bool CollideRight(class GameObject* character, int tile_pos_x, int tile_pos_y);

	// �J�������W�̍X�V
	void UpdateCameraOffset();

	//�X�e�[�W���L�̃f�[�^�̃��[�h�i�w�i�ABGM�j
	void LoadStage(int stage_id);
	//�e�I�u�W�F�N�g�̃X�|�[���ʒu
	std::vector<Vector2D> object_spawn_points;

	// �v���C���[�͕p�ɂɃQ�[������A�N�Z�X�����̂�
	// ���ʂɕێ����Ă���.
	Player* player;
	Vector2D player_respawn_point;

	// ���ݓǂݍ���ł��郌�x���f�[�^
	Stage* stage_data;

	//���݂̃v���C���
	EGamePlayState gameplay_state;

	// �X�e�[�W���n�܂��Ă���̌o�ߎ���.
	// �c�莞�Ԃ̃J�E���g�_�E���Ɏg�p
	float elapsed_time;
	//�X�e�[�W�؂�ւ��܂ł̑ҋ@����
	float stage_waiting_time;
	// UI�֘A
	int ui_handle; 
	int time;			// �c�莞��
	int total_coin;     //�R�C������
	int total_score;    //�X�R�A
	int player_left;	// �v���C���[�c�@
	int current_stage;  //���݂̃X�e�[�W�ԍ��iEStageID�̔ԍ��ɑΉ��j
	int graph_handle_hpbar_base;  //HP�o�[�i�x�[�X�j
	int graph_handle_hpbar_meter;  //HP�o�[�i���[�^�[�j
	int graph_handle_hpbar_flame;  //HP�o�[�i�t���[���j
	int graph_handle_icon_coin;  //�R�C���A�C�R��
	int graph_handle_icon_colon;  //�R���������A�C�R��

	//objects�����X�|�[������I�u�W�F�N�g���n�߂�C���f�b�N�X
	int respawn_object_index;  

	//�T�E���h�֘A
	int sound_handle_stagestart;  //�X�e�[�W�X�^�[�gSE�p
	int sound_handle_stageclear;  //�X�e�[�W�N���ASE�p
	int sound_handle_miss;        //�~�XSE�p
	int sound_handle_1up;       //1UPSE�p
};