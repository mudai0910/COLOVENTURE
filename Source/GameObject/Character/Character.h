#pragma once

#include "../GameObject.h"
#include <vector>

// �����Ă��邩�����Ă��邩
enum class EMovementState : unsigned char
{
	WALKING,
	FALLING,
};

class Character :public GameObject 
{

protected:
	//�A�j���[�V�����X�e�[�g
	enum class EAnimationState :unsigned char
	{
		IDLE,
		WALK,
		JUMP,
		DAMAGE,
		ATTACK,
		DIED,
	};

public:
	Character();
	virtual ~Character();

	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& camera_offset)override;
	virtual void Finalize() override;

	virtual void OnCollision(GameObject* object) override {}
	virtual void ApplyDamage(int amount);


	//�s���~�܂�ɗ�����
	virtual void OnReachDeadEnd()  {}
	//���������ꂩ�痣�ꂽ�Ƃ�
	virtual void OnFallAllFoots() {}

	EAnimationState GetAnimationState() const { return animation_state; }

	//1�߂�const:�߂�l(�|�C���^)�𑼂̊֐��ŕύX���悤�Ƃ���ƃR���p�C���G���[
	//2�߂�const:���g�̊֐����Ń����o�ϐ�������������ƃR���p�C���G���[(�����擾���邽�߂����̊֐�)
	const Vector2D& GetPrevPosition() const
	{
		return prev_position;
	}
	void SetPrevPosition(Vector2D new_prev_position)
	{
		prev_position = new_prev_position;
	}

	void ShiftMovementState(EMovementState new_movement_state);


	int GetHP() const {
		return HP;
	}
	//�����Ă��邩�����Ă��邩
	EMovementState GetMovementState() const { return movement_state; }

protected:
	//�X�e�[�g��ω�������
	void ShiftAnimationState(EAnimationState new_animation_state);
	virtual void OnEnterAnimationState(EAnimationState current_animation_state){}
	virtual void OnLeaveAnimationState(EAnimationState current_animation_state){}
	virtual void OnEnterMovementState(EMovementState current_movement_state) {}
	virtual void OnLeaveMovementState(EMovementState current_movement_state) {}

	virtual void OnDestroy() override;
	virtual void Respawn(Vector2D respawn_point)override;

protected:
	Vector2D graph_offset; //�`�悷��ʒu�̃I�t�Z�b�g�l
	std::vector<std::vector<int>> graphics_handle_buf; 	//�O���t�B�b�N�n���h����2�����x�N�g��
	Vector2D prev_position;  //�O�t���[���̍��W(�R���W�����Ŏg�p)
	int HP;
	bool is_flip_sprite; //�X�v���C�g���]�t���O

	EMovementState movement_state;
	EAnimationState animation_state;

};