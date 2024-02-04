#pragma once
#include "../Utility/Vector2D.h"
#include <string>
#include <vector>
#include "../ObjectTypes.h"


/**
 * �Q�[�����ɕ\�������I�u�W�F�N�g�̊��N���X
 */
class GameObject
{
public:
	GameObject();
	//�f�X�g���N�^
	virtual ~GameObject();

public:
	virtual void Initialize() {}
	virtual void Update(float delta_seconds) {}
	virtual void Draw(const Vector2D& screen_offset) {}
	virtual void Finalize() {}

	//�Փ˔���
	virtual void OnCollision(GameObject* object){}

	//�폜����
	void Destroy();

	//���X�|�[������
	virtual void Respawn(Vector2D respawn_point);

	Vector2D GetPosition() const { return position; }
	void SetPosition(const Vector2D& new_position) { position = new_position; }

	//���x�̏���
	const Vector2D& GetVelocity() const { return velocity; }
	void SetVelocity(Vector2D new_velocity) { velocity = new_velocity; }
	void AddVelocity(const Vector2D& delta_velocity) { velocity = delta_velocity; }

	//�R���W�������擾
	const Vector2D& GetBoxCollision() const { return box_collision; }
	
	//�����蔻�莞�ɃI�u�W�F�N�g�^�C�v���擾
	const EObjectType GetObjectType() const { return object_type; }
	//�폜�\��̃I�u�W�F�N�g���ۂ�
	bool IsDeleteObject() const { return is_delete; }

protected:
	// �폜���̃��A�N�V����
	virtual void OnDestroy();
	// �I�[�i�[�ƂȂ�V�[��
	class SceneBase* owner_scene;
	// �`�揇�B���l��������������`����s��
	int draw_sort_priority;

protected:
	Vector2D position;	// �ʒu
	Vector2D velocity;	// ���x
	Vector2D box_collision;	// �����蔻��{�b�N�X
	bool is_delete;	// �폜�t���O(���t���[�����̃t���O�̓`�F�b�N����Atrue�Ȃ�΍폜�����)

	//�����蔻��p�̃I�u�W�F�N�g�^�C�v�^�O
	EObjectType object_type;
};