#pragma once

#include "../GameObject.h"
#include <vector>

// 立っているか落ちているか
enum class EMovementState : unsigned char
{
	WALKING,
	FALLING,
};

class Character :public GameObject 
{

protected:
	//アニメーションステート
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


	//行き止まりに来た時
	virtual void OnReachDeadEnd()  {}
	//両足が足場から離れたとき
	virtual void OnFallAllFoots() {}

	EAnimationState GetAnimationState() const { return animation_state; }

	//1つめのconst:戻り値(ポインタ)を他の関数で変更しようとするとコンパイルエラー
	//2つめのconst:自身の関数内でメンバ変数を書き換えるとコンパイルエラー(情報を取得するためだけの関数)
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
	//落ちているか立っているか
	EMovementState GetMovementState() const { return movement_state; }

protected:
	//ステートを変化させる
	void ShiftAnimationState(EAnimationState new_animation_state);
	virtual void OnEnterAnimationState(EAnimationState current_animation_state){}
	virtual void OnLeaveAnimationState(EAnimationState current_animation_state){}
	virtual void OnEnterMovementState(EMovementState current_movement_state) {}
	virtual void OnLeaveMovementState(EMovementState current_movement_state) {}

	virtual void OnDestroy() override;
	virtual void Respawn(Vector2D respawn_point)override;

protected:
	Vector2D graph_offset; //描画する位置のオフセット値
	std::vector<std::vector<int>> graphics_handle_buf; 	//グラフィックハンドルの2次元ベクトル
	Vector2D prev_position;  //前フレームの座標(コリジョンで使用)
	int HP;
	bool is_flip_sprite; //スプライト反転フラグ

	EMovementState movement_state;
	EAnimationState animation_state;

};