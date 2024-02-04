#pragma once

#include "../../GameObject.h"

class Ground : public GameObject
{
public:
	Ground();
	virtual ~Ground();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

	//�}�b�v�f�[�^�̃Z�b�g
	void SetMapData(std::vector<std::vector<std::string>> new_map_data) { map_data = new_map_data; }
	//�}�b�v�f�[�^��vector�z��
	std::vector<std::vector<std::string>> map_data;
	//�}�b�v�`�b�v�摜�n���h��,�ŏ��̗v�f��0�ɂ���
	std::vector<int> mapchip_graphics_handles;

private:
	// GR = Ground(�n��), SC = SCAFFOLD(����)
	enum class EMapChipType : unsigned char
	{
		NONE_IMAGE,   // 0:�摜����
		GR_SURFACE_LEFT,	
		GR_SURFACE_CENTER,
		GR_SURFACE_RIGHT,
		GR_MIDDLE_LEFT,
		GR_MIDDLE_CENTER,
		GR_MIDDLE_RIGHT,
		GR_BOTTOM_LEFT,
		GR_BOTTOM_CENTER,
		GR_BOTTOM_RIGHT,  

		SC_IRON_L,	// 10
		SC_WOOD_1_L,
		SC_WOOD_2_L,
		SC_LOG_L,
		SC_IRON_S,
		SC_WOOD_1_S,
		SC_WOOD_2_S,
		SC_LOG_S,

		MAX
	};
};
