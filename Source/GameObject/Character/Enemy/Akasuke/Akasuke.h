#pragma once

#include "../Enemy.h"

class Player;

class Akasuke : public Enemy
{
public:
	Akasuke();
	virtual ~Akasuke();

	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	void OnCollision(GameObject* object) override;

protected:
	void MoveAuto() override;
	void  OnReachDeadEnd() override;
	void Respawn(Vector2D respawn_point) override;

private:
	// �U���J�n������̌o�ߎ���.
	float attack_elapsed_time;
	//�{�f�B�R���W�������ʏ�̃R���W�����͎��씻��Ɏg�p
	Vector2D box_collision_body;
	//�R���W�������s���~�܂�ɂ�����
	bool is_reach_deadend;
	//�v���C���[����������
	bool is_seek_player;
	//�i�s�����ŃR���W�����ƃI�t�Z�b�g�l��ω�������
	const Vector2D BOX_COLLISION= Vector2D(150.0f, 32.0f);
	const Vector2D GRAPH_OFFSET_LEFT= Vector2D(-120.0f, 15.0f);
	const Vector2D GRAPH_OFFSET_RIGHT= Vector2D(5.0f, 15.0f);
	const Vector2D BOX_COLLISION_BODY_LEFT= Vector2D(125.0f, 32.0f);
	const Vector2D BOX_COLLISION_BODY = Vector2D(25.0f, 32.0f);
};

