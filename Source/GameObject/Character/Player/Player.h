#pragma once
#include "../Character.h"

class Player : public Character
{
public:
	Player();
	virtual ~Player();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

	//�R���W��������
	virtual void OnCollision(GameObject* object) override;
	//�_���[�W����
	virtual void ApplyDamage(int amount) override;
	//���G��Ԃ����擾
	bool GetInvincible() const { return is_invincible; }	
	bool GetInvincibleDamaged() const { return is_invincible_damaged; }	

	// �v���C���[���S�t���O/�S�[���t���O
	bool is_player_died;
	bool is_player_goal;

	//�R�C���̖������擾
	int GetCoinNum() { return coin_num; }
	void SetCoinNum(int total_coin_num)
	{
		coin_num = total_coin_num;
	}
	//�X�R�A��ǉ�����
	void AddScore(int amount);
	int GetScore() { return score; }
	void SetScore(int total_score)
	{
		score = total_score;
	}
	//���X�|�[������
	void Respawn(Vector2D respawn_point)override;

	float GetHPRatio() const { return (float)HP / (float)MAX_HP; }

protected:
	virtual void OnEnterAnimationState(EAnimationState current_animation_state) override;
	virtual void OnLeaveAnimationState(EAnimationState current_animation_state) override;
	virtual void OnEnterMovementState(EMovementState current_movement_state)override;
	virtual void OnLeaveMovementState(EMovementState current_movement_state)override;
	virtual void OnDestroy() override;

	virtual void  OnReachDeadEnd()override {};
	virtual void OnFallAllFoots();

private:
	//���͂ƈړ��ʂ̍X�V
	void MoveInput();
	//���W�̍X�V
	void UpdatePosition(float delta_seconds);
	//�A�j���[�V�����t���[���̍X�V
	void UpdateAnimationFrame(float delta_seconds);
	//�A�j���[�V�����X�e�[�g�̍X�V
	void UpdateAnimationState();

private:
	int anim_tick;	// ���݂̃R�}�ɕς���Ă���̌o�߃t���[����
	int anim_speed;	// ���̃R�}��\������܂ł̃t���[����(�������������قǍ����ɃA�j���[�V��������)
	int anim_frame; // ���ݕ\�����̃A�j���[�V�����̃R�}
	int anim_frame_offset; // �\������A�j���[�V�����̃R�}�̃I�t�Z�b�g(����A�j���[�V��������ꕔ�̃t���[���݂̂��Đ����������Ɏg��)
	int anim_frame_max;	// �A�j���[�V�����̃R�}�̍ő吔

	float idle_elapsed_time;	// �A�C�h����ԂɂȂ��Ă���̌o�ߎ���
	float damage_elapsed_time;	// �_���[�W��ԂɂȂ��Ă���̌o�ߎ���
	float fall_seconds;	// �������̌o�ߎ���
	bool is_invincible;				// ���G��Ԃ�
	bool is_invincible_damaged;		// �_���[�W����̖��G��Ԃ�
	float max_invincible_time;		// �ő喳�G����.(��_���[�W���̖��G�ƃA�C�e���擾���̖��G�Ō��ʎ��Ԃ��Ⴄ)
	float invincible_elapsed_time;	// ���G�ɂȂ��Ă���̌o�ߎ���
	bool is_flickering;				// �����ON
	float flickering_elapsed_time;	// �_�ŃG�t�F�N�g���n�܂��Ă���̌o�ߎ���


	const int ANIM_IDLE_TOTAL_FRAME_NUM = 4;
	const int ANIM_MOVE_TOTAL_FRAME_NUM = 8;
	const int ANIM_JUMP_TOTAL_FRAME_NUM = 6;
	const int ANIM_JUMP_FRAME_NUM = 2;

	const int ANIM_JUMP_START_OFFSET = 0;	// ��яオ��
	const int ANIM_JUMP_LOOP_OFFSET = 2;	// ��ђ�
	const int ANIM_JUMP_END_OFFSET = 4;		// ���n

	int coin_num;		// �l���R�C������
	int score;			// ���݃X�R�A
	const int MAX_HP = 50;  //�v���C���[�̍ő�HP
	//SE
	int sound_handle_jump;  //�W�����vSE�p
	int sound_handle_invincible;  //���GSE�p
	int sound_handle_damage;  //�_���[�WSE�p
	int sound_handle_coin;  //�R�C��SE�p

};