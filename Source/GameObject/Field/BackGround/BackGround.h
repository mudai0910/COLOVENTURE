#pragma once
#include "../../GameObject.h"

class BackGround : public GameObject
{
public:
	BackGround();
	virtual ~BackGround();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface
	//�w�i�摜�����[�h���邽�߂̊֐�
	void LoadGraphics(std::string background_graphic_path);
	void SetDistanceScale(const float new_distance_scale) { distance_scale = new_distance_scale; }

private:
	int background_graphic_handle;

	// (�J�����E�w�i���ʂ̋���) / (�J�����E�X�e�[�W���ʂ̋���)
	float distance_scale = 25.f;

	// �w�i�̌J��Ԃ��P�ʂ�3�����ɕ��ׂĕ`�悷��
	Vector2D left_panel_origin;
	Vector2D center_panel_origin;
	Vector2D right_panel_origin;

	//�w�i�̃X�N���[���I�t�Z�b�g
	Vector2D background_screen_offset;

	void DrawBackGround(const Vector2D& screen_offset);
};
