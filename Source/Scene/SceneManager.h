#pragma once

// シーンタイプの前方宣言
enum class ESceneType : unsigned short;

/**
 * 各シーンを管理するクラス
 * シーンの生成/破棄や変更を行う
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
	// 現在のシーン
	class SceneBase* current_scene;
};