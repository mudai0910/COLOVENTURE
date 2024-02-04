#include "Enemy.h"
#include "DxLib.h"
#include "../Player/Player.h"
#include "../../../Manager/SoundManager/SoundManager.h"

Enemy::Enemy()
	:Character()
{
	anim_tick = 0;
	anim_speed = 7;
	anim_frame = 0;
	anim_frame_offset = 0;
	anim_frame_max = 0;
	died_elapsed_time = 0;
	is_turnright = false;
	sound_handle_damage = 0;
	sound_handle_invincibleattack = 0;
	sound_handle_jumpattack = 0;
	walk_speed = 30.0f;
}
Enemy::~Enemy()
{
	Finalize();
}

void Enemy::Initialize()
{
	__super::Initialize();

	object_type = EObjectType::ENEMY;
	box_collision = Vector2D(25, 32);
	ShiftAnimationState(EAnimationState::WALK);
	ShiftMovementState(EMovementState::WALKING);
	sound_handle_damage = SoundManager::GetInstance().LoadSound("Resources/Sound/damage.wav");
	sound_handle_invincibleattack = SoundManager::GetInstance().LoadSound("Resources/Sound/invincibleattack.wav");
	sound_handle_jumpattack = SoundManager::GetInstance().LoadSound("Resources/Sound/jumpattack.wav");
	graph_offset=Vector2D(5.0f, 15.0f);
}
void Enemy::Update(float delta_seconds)
{
	__super::Update(delta_seconds);

	//�ړ�����
	if (GetAnimationState() == EAnimationState::WALK)
	{
		MoveAuto();
	}
	//���W�X�V
	UpdatePosition(delta_seconds);
	//�A�j���[�V�����t���[���̍X�V
	UpdateAnimationFrame(delta_seconds);
	//�A�j���[�V�����X�e�[�g�̍X�V
	UpdateAnimationState();

}

void Enemy::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawReverseGraph(position.x - graph_offset.x - screen_offset.x, position.y - graph_offset.y - screen_offset.y,
		graphics_handle_buf[(int)GetAnimationState()][anim_frame], true, is_flip_sprite);
}

void Enemy::OnEnterAnimationState(EAnimationState current_animation_state)
{
	anim_tick = 0;
	anim_frame = 0;
	switch (current_animation_state)
	{
	case EAnimationState::IDLE:
		anim_frame_max = ANIM_IDLE_TOTAL_FRAME_NUM;
		break;
	case EAnimationState::WALK:
		anim_frame_max = ANIM_WALK_TOTAL_FRAME_NUM;
		break;
	case EAnimationState::DAMAGE:
		anim_frame_max = ANIM_DAMAGE_TOTAL_FRAME_NUM;
		break;
	case EAnimationState::ATTACK:
		anim_frame_max = ANIM_ATTACK_TOTAL_FRAME_NUM;
		break;
	case EAnimationState::DIED:
		anim_frame_max = ANIM_DIED_TOTAL_FRAME_NUM;
		break;
	}
}

void Enemy::OnEnterMovementState(EMovementState current_movement_state)
{
	switch (current_movement_state)
	{
	case EMovementState::WALKING:
		break;
	case EMovementState::FALLING:
		break;
	}
}

void Enemy::OnLeaveMovementState(EMovementState mode)
{
	switch (mode)
	{
	case EMovementState::WALKING:
		break;
	case EMovementState::FALLING:
		break;
	}
}

//�_���[�W��^����OR�_���[�W���󂯂�
void Enemy::OnCollision(GameObject* object)
{
	if (object->GetObjectType() == EObjectType::PLAYER&& GetAnimationState() != EAnimationState::DIED)
	{
		Player* player = dynamic_cast<Player*>(object);

		float player_bottom = object->GetPosition().y + object->GetBoxCollision().y;
		Vector2D center = position + box_collision * 0.5f;
		Vector2D object_center = object->GetPosition() + object->GetBoxCollision() * 0.5f;
		Vector2D to_object = (object_center - center).Normalize();

		if(player->GetInvincible())  // �v���C���[�͖��G���
		{
			//SE
			PlaySoundMem(sound_handle_invincibleattack, DX_PLAYTYPE_BACK, TRUE);
			//�m�b�N�o�b�N
			velocity = Vector2D(to_object.x * -500.0f, -400.0f);

			player->AddScore(500);

			ShiftAnimationState(EAnimationState::DAMAGE);
		}
		else if (player_bottom < position.y + box_collision.y * 0.5f&&GetAnimationState()!= EAnimationState::DAMAGE)	// �v���C���[�ɓ��܂ꂽ
		{
			//SE
			PlaySoundMem(sound_handle_jumpattack, DX_PLAYTYPE_BACK, TRUE);
			player->AddScore(500);	//�X�R�A���Z
			player->SetVelocity(Vector2D(player->GetVelocity().x,-2000.0f));	//���܂ꂽ��v���C���[�͔��
			player->ShiftMovementState(EMovementState::FALLING);
			ShiftAnimationState(EAnimationState::DAMAGE);
		}
		else if(player->GetInvincibleDamaged()==false)  //�_���[�W���󂯂�����łȂ���΃v���C���[�Ƀ_���[�W
		{
			player->ApplyDamage(25);
			//SE
			PlaySoundMem(sound_handle_damage, DX_PLAYTYPE_BACK, TRUE);
			//�v���C���[�Ƀm�b�N�o�b�N��^����
			player->SetVelocity(Vector2D(to_object.x * 500.0f, -100.0f));
		}
	}
}

//�{�X�ȂǁA���̍U�����ᎀ�ȂȂ��G�Ɏg���\��
void Enemy::ApplyDamage(int amount)
{
	//HP -= amount;
	//HP = (HP > 0) ? HP : 0;
	//HP = (HP < 100) ? HP : 100;

	//if (HP <= 0)
	//{
	//	game_play->OnEnemyDied();
	//	return;
	//}

	// ���̒l���^������Ɖ񕜂��Ӗ�����.
	if (amount < 0)
		return;
	ShiftAnimationState(EAnimationState::DAMAGE);
}
//�s���~�܂�ň����Ԃ�
void Enemy::OnReachDeadEnd()
{
	is_turnright = !is_turnright;
}

void Enemy::MoveAuto()
{
	//�[����[�܂ňړ�
	if (is_turnright)
	{
		velocity.x += walk_speed;
		is_flip_sprite = true;
	}
	else
	{
		velocity.x -= walk_speed;
		is_flip_sprite = false;
	}
}

void Enemy::UpdatePosition(float delta_seconds)
{
	prev_position = position;
	position += velocity * delta_seconds;
	if (GetAnimationState() == EAnimationState::DIED)
	{
		velocity *= 0.9f;
	}
	else
	{
		velocity *= 0.0f;
	}
}

void Enemy::UpdateAnimationFrame(float delta_seconds)
{
	// �A�j���[�V�����̍X�V
	anim_tick++;

	if (anim_tick >= anim_speed)
	{
		anim_tick = 0;
		anim_frame = (anim_frame + 1) % anim_frame_max;
	}

	//���΂炭�_���[�W��Ԃ��L�[�v
	if (GetAnimationState() == EAnimationState::DAMAGE)
	{
		died_elapsed_time += delta_seconds;
	}
}
void Enemy::UpdateAnimationState()
{
	// �e�A�N�V�����̑J�ڏ������Ď�.
	switch (GetAnimationState())
	{
	case EAnimationState::IDLE:
		break;
	case EAnimationState::WALK:
		break;
	case EAnimationState::DAMAGE:
		if (died_elapsed_time > 0.2f)  //�����o������J��
		{
			ShiftAnimationState(EAnimationState::DIED);
		}
		break;
	case EAnimationState::ATTACK:  //�A�j���[�V�����I����Walk�ɑJ��
		if (anim_frame >= anim_frame_max - 1)
		{
			ShiftAnimationState(EAnimationState::WALK);
		}
		break;
	case EAnimationState::DIED:  //�A�j���[�V�����I���������
		if (anim_frame >=anim_frame_max-1)
		{
			Destroy();
		}
		break;
	}

}
//���X�|�[��
void Enemy::Respawn(Vector2D respawn_point)
{
	SetPosition(respawn_point);
	ShiftAnimationState(EAnimationState::WALK);
	died_elapsed_time = 0;
	is_turnright = false;
}

void Enemy::Finalize()
{
	__super::Finalize();
}