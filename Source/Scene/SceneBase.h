#pragma once

#include <vector>
#include <string>
#include "../Utility/Vector2D.h"
#include "../GameObject/GameObject.h"

/**
 * シーンタイプ
 */
enum class ESceneType : unsigned short
{
	TITLE_SCENE,
	INGAME_SCENE,
	GAMEOVER_SCENE,
	GAMECLEAR_SCENE
};

/**
 * シーンの基底クラス
 */
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase();

public:
	/**
	 * 初期化
	 */
	virtual void Initialize();

	/**
	 * 更新
	 * @param	delta_seconds	前フレームとの差分時間(s)
	 * @return	次に遷移するシーンタイプ
	 */
	virtual ESceneType Update(float delta_seconds);

	/** 
	 * 描画
	 */
	virtual void Draw();

	/**
	 * 解放
	 */
	virtual void Finalize();

	/**
	 * シーンタイプの取得
	 * @return シーンタイプ
	 */
	virtual ESceneType GetSceneType() const = 0;

	/**
	 * GameObjectの生成
	 * Templateで指定したGameObjectクラスを派生したクラスのインスタンス生成する
	 * @param	Position	初期位置
	 * @return	生成したGameObject
	 */
	template <class T>
	T* CreateObject(const Vector2D& position)
	{
		// GameObjectの生成
		T* new_instance = new T();
		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		// GameObjectを派生していない場合は、破棄して終了する
		if (new_object == nullptr)
		{
			//派生していないクラスのため、生成しない
			delete new_instance;
			return nullptr;
		}

		// GameObjectの初期化
		new_object->SetPosition(position);
		new_object->Initialize();
		objects.push_back(new_object);

		return new_instance;
	}

	/**
	 * 全てのGameObjectの破棄
	 * シーンに生成されている全てのオブジェクトを破棄する
	 */
	void DestroyAllObjects();

protected:
	// シーンで生成したオブジェクト
	std::vector<class GameObject*> objects;

	//BGM用
	int sound_handle;


	// スクロール用スクリーンオフセット
	Vector2D screen_offset;
};