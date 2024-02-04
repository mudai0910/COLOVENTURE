#pragma once

#include <vector>
#include <string>
#include "../Utility/Vector2D.h"
#include "../GameObject/GameObject.h"

/**
 * �V�[���^�C�v
 */
enum class ESceneType : unsigned short
{
	TITLE_SCENE,
	INGAME_SCENE,
	GAMEOVER_SCENE,
	GAMECLEAR_SCENE
};

/**
 * �V�[���̊��N���X
 */
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase();

public:
	/**
	 * ������
	 */
	virtual void Initialize();

	/**
	 * �X�V
	 * @param	delta_seconds	�O�t���[���Ƃ̍�������(s)
	 * @return	���ɑJ�ڂ���V�[���^�C�v
	 */
	virtual ESceneType Update(float delta_seconds);

	/** 
	 * �`��
	 */
	virtual void Draw();

	/**
	 * ���
	 */
	virtual void Finalize();

	/**
	 * �V�[���^�C�v�̎擾
	 * @return �V�[���^�C�v
	 */
	virtual ESceneType GetSceneType() const = 0;

	/**
	 * GameObject�̐���
	 * Template�Ŏw�肵��GameObject�N���X��h�������N���X�̃C���X�^���X��������
	 * @param	Position	�����ʒu
	 * @return	��������GameObject
	 */
	template <class T>
	T* CreateObject(const Vector2D& position)
	{
		// GameObject�̐���
		T* new_instance = new T();
		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		// GameObject��h�����Ă��Ȃ��ꍇ�́A�j�����ďI������
		if (new_object == nullptr)
		{
			//�h�����Ă��Ȃ��N���X�̂��߁A�������Ȃ�
			delete new_instance;
			return nullptr;
		}

		// GameObject�̏�����
		new_object->SetPosition(position);
		new_object->Initialize();
		objects.push_back(new_object);

		return new_instance;
	}

	/**
	 * �S�Ă�GameObject�̔j��
	 * �V�[���ɐ�������Ă���S�ẴI�u�W�F�N�g��j������
	 */
	void DestroyAllObjects();

protected:
	// �V�[���Ő��������I�u�W�F�N�g
	std::vector<class GameObject*> objects;

	//BGM�p
	int sound_handle;


	// �X�N���[���p�X�N���[���I�t�Z�b�g
	Vector2D screen_offset;
};