#pragma once

// �V�[���^�C�v�̑O���錾
enum class ESceneType : unsigned short;

/**
 * �e�V�[�����Ǘ�����N���X
 * �V�[���̐���/�j����ύX���s��
 */
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

public:
	void Initialize();
	void Update(float delta_seconds);
	void Finalize();

private:
	void Draw();
	void ChangeScene(ESceneType new_scene_type);
	class SceneBase* CreateScene(ESceneType new_scene_type);

private:
	// ���݂̃V�[��
	class SceneBase* current_scene;
};