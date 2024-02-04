#include "Player.h"
#include "DxLib.h"
#include "../../../SystemTypes.h"
#include "../../../Manager/InputManager/InputManager.h"
#include "../../../Manager/GraphicsManager/GraphicsManager.h"
#include "../../../Manager/SoundManager/SoundManager.h"
//�R���X�g���N�^
Player::Player()
	:Character()
{
	object_type = EObjectType::PLAYER;
	anim_tick = 0;
	anim_speed = 5;
	anim_frame = 0;
	idle_elapsed_time = 0;
	damage_elapsed_time = 0;
	anim_frame_offset = 0;
	anim_frame_max=0;
	fall_seconds = 0.0f;	
	is_invincible = false;
	is_invincible_damaged = false;
	max_invincible_time = 1.0f;
	invincible_elapsed_time=0.0f;
	is_flickering=false;
	flickering_elapsed_time = 0.0f;
	is_player_died=false;
	is_player_goal=false;
	coin_num = 0;
	score = 0;
	sound_handle_jump = 0;
	sound_handle_invincible = 0;
	sound_handle_damage = 0;
	sound_handle_coin = 0;

	HP = MAX_HP;

}
//�f�X�g���N�^
Player::~Player()
{
	Finalize();
}

void Player::Initialize()
{
	__super::Initialize();

	InputManager::GetInstance().Initialize();
	graphics_handle_buf.resize((int)EAnimationState::DIED + 1);

	graphics_handle_buf[(int)EAnimationState::IDLE].resize(ANIM_IDLE_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/collon_wait_a.bmp"),
		ANIM_IDLE_TOTAL_FRAME_NUM, 4, 1, 128, 128,
		graphics_handle_buf[(int)EAnimationState::IDLE]);

	graphics_handle_buf[(int)EAnimationState::WALK].resize(ANIM_MOVE_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/collon_run8.bmp"),
		ANIM_MOVE_TOTAL_FRAME_NUM, 4, 2, 128, 128,
		graphics_handle_buf[(int)EAnimationState::WALK]);

	graphics_handle_buf[(int)EAnimationState::JUMP].resize(ANIM_JUMP_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/collon_jump.bmp"),
		ANIM_JUMP_TOTAL_FRAME_NUM, 4, 2, 128, 128,
		graphics_handle_buf[(int)EAnimationState::JUMP]);

	//�_���[�W�A�j���[�V�����Ǝ��S�A�j���[�V�����͋���
	int handle = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/collon_damage.bmp"));
	graphics_handle_buf[(int)EAnimationState::DAMAGE].push_back(handle);
	graphics_handle_buf[(int)EAnimationState::DIED].push_back(handle);

	box_collision = Vector2D(25, 50);
	ShiftAnimationState(EAnimationState::IDLE);
	ShiftMovementState(EMovementState::FALLING);
	//�摜�̃I�t�Z�b�g�l
	graph_offset=Vector2D(52.0f, 57.0f);

	sound_handle_jump = SoundManager::GetInstance().LoadSound("Resources/Sound/jump.wav");
	sound_handle_invincible = SoundManager::GetInstance().LoadSound("Resources/Sound/invincible.wav");
	sound_handle_damage = SoundManager::GetInstance().LoadSound("Resources/Sound/damage.wav");
	sound_handle_coin = SoundManager::GetInstance().LoadSound("Resources/Sound/coin.wav");
}

void Player::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	
	//�ړ�����
	MoveInput();
	//�A�j���[�V�����t���[���̍X�V
	UpdateAnimationFrame(delta_seconds);
	//�A�j���[�V�����X�e�[�g�̍X�V
	UpdateAnimationState();
	//���W�X�V
	UpdatePosition(delta_seconds);

	// ���G��Ԃ̏���
	if (is_invincible|| is_invincible_damaged)
	{
		// ���G��ԊJ�n������̌o�ߎ��Ԃ��v�Z
		invincible_elapsed_time += delta_seconds;
		if (invincible_elapsed_time >= max_invincible_time)
		{
			// ���G���ԏI��
			invincible_elapsed_time = 0.f;
			is_invincible = false;
			is_invincible_damaged = false;
			is_flickering = false;
			return;
		}
		// ���_�ŃG�t�F�N�g�̍X�V
		flickering_elapsed_time += delta_seconds;
		if (flickering_elapsed_time > 0.04f)
		{
			flickering_elapsed_time = 0.0f;
			is_flickering = !is_flickering;
		}
	}
	//���S�t���O�𗧂Ă�
	if (HP <= 0&&GetAnimationState()!=EAnimationState::DAMAGE)
	{
		Destroy();
	}
}

void Player::Draw(const Vector2D& screen_offset)
{
	__super::Draw(screen_offset);

	//const Vector2D graph_offset(52.0f, 57.0f);

	if (!is_flickering)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawReverseGraph(position.x - graph_offset.x - screen_offset.x, position.y - graph_offset.y - screen_offset.y, graphics_handle_buf[(int)GetAnimationState()][anim_frame + anim_frame_offset], true, is_flip_sprite);
	}
	else
	{
		// ���G���̔��_�ŃG�t�F�N�g(�u�����h���[�h���g���ē����摜���d�˂ĕ`��)
		// �F�𔽓]�����ĕ`��
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 200);
		DrawReverseGraph(position.x - graph_offset.x - screen_offset.x, position.y - graph_offset.y - screen_offset.y, graphics_handle_buf[(int)GetAnimationState()][anim_frame + anim_frame_offset], true, is_flip_sprite);
		// ��ŕ`�悳�ꂽ�摜�̏�ɐV�����摜�����Z�ō�������.
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawReverseGraph(position.x - graph_offset.x - screen_offset.x, position.y - graph_offset.y - screen_offset.y, graphics_handle_buf[(int)GetAnimationState()][anim_frame + anim_frame_offset], true, is_flip_sprite);
		// ���̃u�����h���[�h�ɖ߂�(���ɂ��e�����Ă��܂�)
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
}

void Player::OnEnterAnimationState(EAnimationState current_animation_state)
{
	anim_tick = 0;
	anim_frame = 0;
	switch (current_animation_state)
	{
	case EAnimationState::IDLE:
		anim_frame_max = ANIM_IDLE_TOTAL_FRAME_NUM;
		break;
	case EAnimationState::WALK:
		anim_frame_max = ANIM_MOVE_TOTAL_FRAME_NUM;
		break;
	case EAnimationState::JUMP:
		anim_frame_max = ANIM_JUMP_FRAME_NUM;
		ShiftMovementState(EMovementState::FALLING);
		break;
	case EAnimationState::DAMAGE:
		anim_frame_max = 1;
		break;
	case EAnimationState::DIED:
		break;
	}
}

void Player::OnLeaveAnimationState(EAnimationState current_animation_state)
{
	switch (current_animation_state)
	{
	case EAnimationState::IDLE:
		break;
	case EAnimationState::WALK:
		break;
	case EAnimationState::JUMP:
		anim_frame_offset = 0;
		break;
	case EAnimationState::DAMAGE:
		damage_elapsed_time =0;
		break;
	case EAnimationState::DIED:
		break;
	}
}

void Player::OnEnterMovementState(EMovementState current_movement_state)
{
	switch (current_movement_state)
	{
	case EMovementState::WALKING:
		break;
	case EMovementState::FALLING:
		fall_seconds = 0.0f;
		break;
	}
}

void Player::OnLeaveMovementState(EMovementState mode)
{
	switch (mode)
	{
	case EMovementState::WALKING:
		break;
	case EMovementState::FALLING:
		ShiftAnimationState(EAnimationState::IDLE);
		break;
	}
}
//�e�I�u�W�F�N�g�Ƃ̏Փˏ���
void Player::OnCollision(GameObject* object)
{
	switch (object->GetObjectType())
	{
	case EObjectType::ENEMY:
		if (GetAnimationState() == EAnimationState::DAMAGE)
		{
			break;
		}
		// �G�𓥂݂���
		if(!(is_invincible)&&  //���G�ł͂Ȃ�
			position.y + box_collision.y < object->GetPosition().y + object->GetBoxCollision().y * 0.5f)  //�������ł͂Ȃ�
		{
			//ShiftAnimationState(EAnimationState::JUMP);
		}
		break;
	case EObjectType::ENERGYDRINK:
		PlaySoundMem(sound_handle_invincible, DX_PLAYTYPE_BACK, TRUE);
		max_invincible_time = 10.0f;
		is_invincible = true;
		//���G�h�����N�擾�őS��
		ApplyDamage(-MAX_HP);
		break;
	case EObjectType::COIN:
		coin_num++;
		AddScore(100);
		//SE
		PlaySoundMem(sound_handle_coin, DX_PLAYTYPE_BACK, TRUE);
		break;
	case EObjectType::GOALFLAG:
		is_player_goal = true;
		break;
	}
}

void Player::ApplyDamage(int amount)
{
	// ���G���̓_���[�W���������s���Ȃ�.
	if (is_invincible && amount > 0)
	{
		return;
	}
	//HP�̍ő�l�ƍŏ��l�𒴂��Ȃ��悤�ɂ���
	HP -= amount;
	HP = (HP > 0) ? HP : 0;
	HP = (HP < MAX_HP) ? HP : MAX_HP;

	// ���̒l���^������Ɖ񕜂��Ӗ�����.
	// �񕜂̓m�b�N�o�b�N��_���[�W��ԂɑJ�ڂ���K�v�͂Ȃ��̂ŁA�����ŏI���.
	if (amount < 0)
	{
		return;
	}

	// �_���[�W���󂯂��疳�G��ԂɂȂ�.
	max_invincible_time = 1.f;
	is_invincible_damaged = true;

	ShiftAnimationState(EAnimationState::DAMAGE);
}

//������������Ɨ�����iCharacter�N���X����Override�j
void Player::OnFallAllFoots()
{
	ShiftMovementState(EMovementState::FALLING);
}


void Player::MoveInput()
{
	const float SPEED = 50.0f;
	if (InputManager::GetInstance().GetKey(KEY_INPUT_A) || InputManager::GetInstance().GetKey(KEY_INPUT_LEFT))
	{
		velocity.x -= SPEED;
		is_flip_sprite = true;
	}
	if (InputManager::GetInstance().GetKey(KEY_INPUT_D)||InputManager::GetInstance().GetKey(KEY_INPUT_RIGHT))
	{
		velocity.x += SPEED;
		is_flip_sprite = false;
	}
	//�W�����v�̓L�[���������u�Ԃ݂̂��n�ʂɂ���ꍇ�̂ݎ��s
	if (InputManager::GetInstance().GetKeyDown(KEY_INPUT_SPACE) && GetMovementState() != EMovementState::FALLING)
	{
		velocity.y = -1500.0f;
		//SE
		PlaySoundMem(sound_handle_jump, DX_PLAYTYPE_BACK, TRUE);
		ShiftAnimationState(EAnimationState::JUMP);
	}

	InputManager::GetInstance().Update();
}

void Player::UpdatePosition(float delta_seconds)
{
	// �d�͂̓K�p
	if (GetMovementState() == EMovementState::FALLING)
	{
		velocity.y += 0.5f * 300.0f * fall_seconds *fall_seconds;
		velocity.y = (velocity.y < 500.0f) ? velocity.y : 500.0f;
		fall_seconds += delta_seconds;
	}

	prev_position = position;
	position += velocity * delta_seconds;

	//���x�̌���
	velocity *= 0.9f;
	if (abs(velocity.x)< 12.0f)
	{
		velocity.x *= 0.1f;
	}
	// ����������
	if (position.y > SCREEN_RESOLUTION_Y)
	{
		Destroy();
	}
}

void Player::UpdateAnimationFrame(float delta_seconds)
{
	if (GetAnimationState() == EAnimationState::JUMP)
	{
		// �㉺�̑��x�����čĐ�����W�����v�A�j���[�V������ω�������(��яオ��->��ђ�->���n)
		if (velocity.y < -150.0f)
			anim_frame_offset = ANIM_JUMP_START_OFFSET;
		else if (velocity.y >= -150.0f && velocity.y <= 150.0f)
			anim_frame_offset = ANIM_JUMP_LOOP_OFFSET;
		else if (velocity.y > 150.0f)
			anim_frame_offset = ANIM_JUMP_END_OFFSET;
	}
	else if (GetAnimationState() == EAnimationState::DAMAGE)
	{
		//���΂炭�_���[�W��Ԃ��L�[�v
		damage_elapsed_time += delta_seconds;
	}
	else if (GetAnimationState() == EAnimationState::IDLE)
	{
		idle_elapsed_time += delta_seconds;

		// 3�b�ȏ�o����.
		if (idle_elapsed_time > 3.0f)
		{
			// �ҋ@�A�j���[�V�����J�n.
			anim_tick++;

			if (anim_tick >= anim_speed)
			{
				anim_tick = 0;
				anim_frame = (anim_frame + 1) % ANIM_IDLE_TOTAL_FRAME_NUM;
				// �A�j���[�V�������P������.
				if (anim_frame == 0)
				{
					idle_elapsed_time = 0.0f;
				}
			}
		}
		return;
	}
	// �A�j���[�V�����̍X�V
	anim_tick++;
	if (anim_tick >= anim_speed)
	{
		anim_tick = 0;
		anim_frame = (anim_frame + 1) % anim_frame_max;
	}
}

void Player::UpdateAnimationState()
{
	// �e�A�N�V�����̑J�ڏ������Ď�.
	switch (GetAnimationState())
	{
	case EAnimationState::IDLE:
		if (std::abs(velocity.x) > 10.0f)
			ShiftAnimationState(EAnimationState::WALK);
		break;
	case EAnimationState::WALK:
		if (std::abs(velocity.x) <= 10.0f)
			ShiftAnimationState(EAnimationState::IDLE);
		break;
	case EAnimationState::JUMP:
		break;
	case EAnimationState::DAMAGE:
		if (damage_elapsed_time > 3.0f)
		{
			damage_elapsed_time = 0;
			ShiftAnimationState(EAnimationState::IDLE);
		}
		break;
	case EAnimationState::DIED:
		break;
	}
}

void Player::OnDestroy()
{
	ShiftAnimationState(EAnimationState::DAMAGE);
	//���S�����t���O�𗧂Ă�
	is_player_died = true;
}

void Player::AddScore(int amount)
{
	score += amount;
}

void Player::Respawn(Vector2D respawn_point)
{
	ShiftAnimationState(EAnimationState::IDLE);
	//���X�|�[��
	SetPosition(respawn_point);
	//HP��MAX�ɖ߂�
	HP = MAX_HP;
	is_player_died = false;
	is_flickering = false;
	is_invincible = false;
	is_invincible_damaged = false;
}

void Player::Finalize()
{
	__super::Finalize();
}
