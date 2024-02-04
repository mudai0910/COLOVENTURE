#include "Akasuke.h"
#include <DxLib.h>
#include <cmath>
#include "../../../../Manager/GraphicsManager/GraphicsManager.h"
#include "../../Player/Player.h"

Akasuke::Akasuke()
	: Enemy()
{
	attack_elapsed_time=0;
	is_reach_deadend = false;
	is_seek_player = false;
}

Akasuke::~Akasuke()
{
}

void Akasuke::Initialize()
{
	__super::Initialize();

	//����͈͂��R���W�����ɂ���
	box_collision = BOX_COLLISION;
	//���������̕`��I�t�Z�b�g�l
	graph_offset = GRAPH_OFFSET_LEFT;
	//���������̃{�f�B�R���W����
	box_collision_body = BOX_COLLISION_BODY_LEFT;


	graphics_handle_buf.resize((int)EAnimationState::DIED + 1);

	graphics_handle_buf[(int)EAnimationState::IDLE].resize(ANIM_IDLE_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_rd_wait.bmp"),
		ANIM_IDLE_TOTAL_FRAME_NUM, ANIM_IDLE_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::IDLE]);

	graphics_handle_buf[(int)EAnimationState::WALK].resize(ANIM_WALK_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_rd_walk.bmp"),
		ANIM_WALK_TOTAL_FRAME_NUM, ANIM_WALK_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::WALK]);

	graphics_handle_buf[(int)EAnimationState::DAMAGE].resize(ANIM_DAMAGE_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_rd_damage.bmp"),
		ANIM_DAMAGE_TOTAL_FRAME_NUM, ANIM_DAMAGE_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::DAMAGE]);

	graphics_handle_buf[(int)EAnimationState::ATTACK].resize(ANIM_ATTACK_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_rd_attack.bmp"),
		ANIM_ATTACK_TOTAL_FRAME_NUM, ANIM_ATTACK_TOTAL_FRAME_NUM, 1, 56, 48,
		graphics_handle_buf[(int)EAnimationState::ATTACK]);

	graphics_handle_buf[(int)EAnimationState::DIED].resize(ANIM_DIED_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/enemy_death.bmp"),
		ANIM_DIED_TOTAL_FRAME_NUM, ANIM_DIED_TOTAL_FRAME_NUM, 1, 40, 40,
		graphics_handle_buf[(int)EAnimationState::DIED]);

	ShiftAnimationState(EAnimationState::WALK);
	ShiftMovementState(EMovementState::WALKING);
}

void Akasuke::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	//�v���C���[�T�m�t���O�͏��false�ɂ��Ă����i�R���W�������ɂ����true�ɏ㏑�������j
	is_seek_player = false;

}

//�v���C���[�����E�ɓ������ꍇ
void Akasuke::OnCollision(GameObject* object)
{
	if (object->GetObjectType() == EObjectType::PLAYER && GetAnimationState() != EAnimationState::DIED)
	{
		//Player* player = dynamic_cast<Player*>(object);

		//�ǂ�������t���O��true
		is_seek_player = true;

		// �A�^�b�N�͈͓�
		if (!(is_turnright) && position.x + box_collision_body.x - BOX_COLLISION_BODY.x < object->GetPosition().x + object->GetBoxCollision().x
			|| (is_turnright) && object->GetPosition().x < position.x + box_collision_body.x + -BOX_COLLISION_BODY.x)
		{
			//�A�^�b�N��Ԃ�������
			if (GetAnimationState() == EAnimationState::ATTACK)
			{
				__super::OnCollision(object);
			}
			else
			{
				ShiftAnimationState(EAnimationState::ATTACK);
			}
		}
		//�{�f�B�R���W������
		if (!(is_turnright)&&position.x+ box_collision_body.x < object->GetPosition().x + object->GetBoxCollision().x
			|| (is_turnright)&& object->GetPosition().x<position.x + box_collision_body.x)
		{
			__super::OnCollision(object);
		}
	}

}

void Akasuke::MoveAuto()
{
	//�v���C���[����������X�s�[�h�A�b�v
	walk_speed = is_seek_player ? 100 : 30;
	if (GetAnimationState() == EAnimationState::WALK)
	{
		__super::MoveAuto();
	}
}
//�s���~�܂�ɓ��B
void Akasuke::OnReachDeadEnd()
{
	if (is_reach_deadend==false) //����͈͂��s���~�܂�ɓ��B������
	{
		//�������̏ꍇposition���{�f�B�R���W������������W�ɂ���
		if(!is_turnright)position.x += BOX_COLLISION_BODY_LEFT.x;

		//�R���W�����̑傫�����{�f�B�R���W�����ɂ���
		box_collision = BOX_COLLISION_BODY;
		box_collision_body = BOX_COLLISION_BODY;
		graph_offset = GRAPH_OFFSET_RIGHT;

		//���ɂ��̊֐����Ă΂��̂̓{�f�B�R���W���������B�����Ƃ�
		is_reach_deadend = true;
	}
	else  //�{�f�B�R���W�������s���~�܂�ɓ��B������
	{
		//�����𔽓]������
		is_turnright = !is_turnright;
		box_collision = BOX_COLLISION;
		//�����Ԃ�
		if (is_turnright) //������E�֌������ꍇ
		{
			graph_offset = GRAPH_OFFSET_RIGHT;
			box_collision_body = BOX_COLLISION_BODY;
		}
		else  //�E���獶�֌������ꍇ
		{
			graph_offset = GRAPH_OFFSET_LEFT;
			box_collision_body = BOX_COLLISION_BODY_LEFT;
			position.x -=  box_collision_body.x;
		}

		is_reach_deadend = false;

	}
}

//���X�|�[��
void Akasuke::Respawn(Vector2D respawn_point)
{
	__super::Respawn(respawn_point);
	attack_elapsed_time = 0;
	is_reach_deadend = false;
	graph_offset = GRAPH_OFFSET_LEFT;
	box_collision_body = BOX_COLLISION_BODY_LEFT;
	box_collision = BOX_COLLISION;

}
