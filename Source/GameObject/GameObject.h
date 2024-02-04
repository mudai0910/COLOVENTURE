#pragma once
#include "../Utility/Vector2D.h"
#include <string>
#include <vector>
#include "../ObjectTypes.h"


/**
 * ゲーム内に表示されるオブジェクトの基底クラス
 */
class GameObject
{
public:
	GameObject();
	//デストラクタ
	virtual ~GameObject();

public:
	virtual void Initialize() {}
	virtual void Update(float delta_seconds) {}
	virtual void Draw(const Vector2D& screen_offset) {}
	virtual void Finalize() {}

	//衝突判定
	virtual void OnCollision(GameObject* object){}

	//削除処理
	void Destroy();

	//リスポーン処理
	virtual void Respawn(Vector2D respawn_point);

	Vector2D GetPosition() const { return position; }
	void SetPosition(const Vector2D& new_position) { position = new_position; }

	//速度の処理
	const Vector2D& GetVelocity() const { return velocity; }
	void SetVelocity(Vector2D new_velocity) { velocity = new_velocity; }
	void AddVelocity(const Vector2D& delta_velocity) { velocity = delta_velocity; }

	//コリジョンを取得
	const Vector2D& GetBoxCollision() const { return box_collision; }
	
	//当たり判定時にオブジェクトタイプを取得
	const EObjectType GetObjectType() const { return object_type; }
	//削除予定のオブジェクトか否か
	bool IsDeleteObject() const { return is_delete; }

protected:
	// 削除時のリアクション
	virtual void OnDestroy();
	// オーナーとなるシーン
	class SceneBase* owner_scene;
	// 描画順。数値が小さい順から描画を行う
	int draw_sort_priority;

protected:
	Vector2D position;	// 位置
	Vector2D velocity;	// 速度
	Vector2D box_collision;	// 当たり判定ボックス
	bool is_delete;	// 削除フラグ(毎フレームこのフラグはチェックされ、trueならば削除される)

	//当たり判定用のオブジェクトタイプタグ
	EObjectType object_type;
};