#pragma once

#include "../Character.h"

class Enemy : public Character
{
public:
	Enemy();
	virtual ~Enemy();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

	virtual void OnCollision(GameObject* object) override;	//�R���W��������
	virtual void ApplyDamage(int amount) override;	//�_���[�W����
	virtual void Respawn(Vector2D respawn_point)override;	//���X�|�[������

protected:
	virtual void OnEnterAnimationState(EAnimationState current_animation_state) override;
	virtual void OnEnterMovementState(EMovementState current_movement_state)override;
	virtual void OnLeaveMovementState(EMovementState current_movement_state)override;
	virtual void  OnReachDeadEnd()override;	//�s���~�܂�ɗ�����

	//�A�j���[�V�����t���[��
	const int ANIM_IDLE_TOTAL_FRAME_NUM = 1;
	const int ANIM_WALK_TOTAL_FRAME_NUM = 4;
	const int ANIM_DAMAGE_TOTAL_FRAME_NUM = 1;
	const int ANIM_ATTACK_TOTAL_FRAME_NUM = 3;
	const int ANIM_DIED_TOTAL_FRAME_NUM = 5;

	int anim_tick;	// ���݂̃R�}�ɕς���Ă���̌o�߃t���[����
	int anim_speed;	// ���̃R�}��\������܂ł̃t���[����(�������������قǍ����ɃA�j���[�V��������)
	int anim_frame; // ���ݕ\�����̃A�j���[�V�����̃R�}
	int anim_frame_offset; // �\������A�j���[�V�����̃R�}�̃I�t�Z�b�g(����A�j���[�V��������ꕔ�̃t���[���݂̂��Đ����������Ɏg��)
	int anim_frame_max;	// �A�j���[�V�����̃R�}�̍ő吔
	float died_elapsed_time;	// DEAD��ԂɂȂ��Ă���̌o�ߎ���
	bool is_turnright;  //�����Ԃ��Ă��邩
	int sound_handle_damage;  //�_���[�WSE�p
	int sound_handle_invincibleattack;  //���G�A�^�b�NSE�p
	int sound_handle_jumpattack;  //�W�����v�A�^�b�NSE�p	
	virtual void MoveAuto();	//�ړ��ʂ̍X�V
	float walk_speed;	//�����X�s�[�h

private:
	//���W�̍X�V
	void UpdatePosition(float delta_seconds);
	//�A�j���[�V�����t���[���̍X�V
	void UpdateAnimationFrame(float delta_seconds);
	//�A�j���[�V�����X�e�[�g�̍X�V
	void UpdateAnimationState();

};
