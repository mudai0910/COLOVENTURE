#pragma once

#include "../Character.h"

class Enemy : public Character
{
public:
	Enemy();
	virtual ~Enemy();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

	virtual void OnCollision(GameObject* object) override;	//コリジョン処理
	virtual void ApplyDamage(int amount) override;	//ダメージ処理
	virtual void Respawn(Vector2D respawn_point)override;	//リスポーン処理

protected:
	virtual void OnEnterAnimationState(EAnimationState current_animation_state) override;
	virtual void OnEnterMovementState(EMovementState current_movement_state)override;
	virtual void OnLeaveMovementState(EMovementState current_movement_state)override;
	virtual void  OnReachDeadEnd()override;	//行き止まりに来た時

	//アニメーションフレーム
	const int ANIM_IDLE_TOTAL_FRAME_NUM = 1;
	const int ANIM_WALK_TOTAL_FRAME_NUM = 4;
	const int ANIM_DAMAGE_TOTAL_FRAME_NUM = 1;
	const int ANIM_ATTACK_TOTAL_FRAME_NUM = 3;
	const int ANIM_DIED_TOTAL_FRAME_NUM = 5;

	int anim_tick;	// 現在のコマに変わってからの経過フレーム数
	int anim_speed;	// 次のコマを表示するまでのフレーム数(数字が小さいほど高速にアニメーションする)
	int anim_frame; // 現在表示中のアニメーションのコマ
	int anim_frame_offset; // 表示するアニメーションのコマのオフセット(あるアニメーションから一部のフレームのみを再生したい時に使う)
	int anim_frame_max;	// アニメーションのコマの最大数
	float died_elapsed_time;	// DEAD状態になってからの経過時間
	bool is_turnright;  //引き返しているか
	int sound_handle_damage;  //ダメージSE用
	int sound_handle_invincibleattack;  //無敵アタックSE用
	int sound_handle_jumpattack;  //ジャンプアタックSE用	
	virtual void MoveAuto();	//移動量の更新
	float walk_speed;	//歩くスピード

private:
	//座標の更新
	void UpdatePosition(float delta_seconds);
	//アニメーションフレームの更新
	void UpdateAnimationFrame(float delta_seconds);
	//アニメーションステートの更新
	void UpdateAnimationState();

};
