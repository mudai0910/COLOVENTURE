#pragma once
#include "../Character.h"

class Player : public Character
{
public:
	Player();
	virtual ~Player();

public:
	//~ Begin GameObject interface
	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;
	//~ End GameObject interface

	//コリジョン処理
	virtual void OnCollision(GameObject* object) override;
	//ダメージ処理
	virtual void ApplyDamage(int amount) override;
	//無敵状態かを取得
	bool GetInvincible() const { return is_invincible; }	
	bool GetInvincibleDamaged() const { return is_invincible_damaged; }	

	// プレイヤー死亡フラグ/ゴールフラグ
	bool is_player_died;
	bool is_player_goal;

	//コインの枚数を取得
	int GetCoinNum() { return coin_num; }
	void SetCoinNum(int total_coin_num)
	{
		coin_num = total_coin_num;
	}
	//スコアを追加する
	void AddScore(int amount);
	int GetScore() { return score; }
	void SetScore(int total_score)
	{
		score = total_score;
	}
	//リスポーン処理
	void Respawn(Vector2D respawn_point)override;

	float GetHPRatio() const { return (float)HP / (float)MAX_HP; }

protected:
	virtual void OnEnterAnimationState(EAnimationState current_animation_state) override;
	virtual void OnLeaveAnimationState(EAnimationState current_animation_state) override;
	virtual void OnEnterMovementState(EMovementState current_movement_state)override;
	virtual void OnLeaveMovementState(EMovementState current_movement_state)override;
	virtual void OnDestroy() override;

	virtual void  OnReachDeadEnd()override {};
	virtual void OnFallAllFoots();

private:
	//入力と移動量の更新
	void MoveInput();
	//座標の更新
	void UpdatePosition(float delta_seconds);
	//アニメーションフレームの更新
	void UpdateAnimationFrame(float delta_seconds);
	//アニメーションステートの更新
	void UpdateAnimationState();

private:
	int anim_tick;	// 現在のコマに変わってからの経過フレーム数
	int anim_speed;	// 次のコマを表示するまでのフレーム数(数字が小さいほど高速にアニメーションする)
	int anim_frame; // 現在表示中のアニメーションのコマ
	int anim_frame_offset; // 表示するアニメーションのコマのオフセット(あるアニメーションから一部のフレームのみを再生したい時に使う)
	int anim_frame_max;	// アニメーションのコマの最大数

	float idle_elapsed_time;	// アイドル状態になってからの経過時間
	float damage_elapsed_time;	// ダメージ状態になってからの経過時間
	float fall_seconds;	// 落下中の経過時間
	bool is_invincible;				// 無敵状態か
	bool is_invincible_damaged;		// ダメージ直後の無敵状態か
	float max_invincible_time;		// 最大無敵時間.(被ダメージ時の無敵とアイテム取得時の無敵で効果時間が違う)
	float invincible_elapsed_time;	// 無敵になってからの経過時間
	bool is_flickering;				// 白飛びON
	float flickering_elapsed_time;	// 点滅エフェクトが始まってからの経過時間


	const int ANIM_IDLE_TOTAL_FRAME_NUM = 4;
	const int ANIM_MOVE_TOTAL_FRAME_NUM = 8;
	const int ANIM_JUMP_TOTAL_FRAME_NUM = 6;
	const int ANIM_JUMP_FRAME_NUM = 2;

	const int ANIM_JUMP_START_OFFSET = 0;	// 飛び上がり
	const int ANIM_JUMP_LOOP_OFFSET = 2;	// 飛び中
	const int ANIM_JUMP_END_OFFSET = 4;		// 着地

	int coin_num;		// 獲得コイン枚数
	int score;			// 現在スコア
	const int MAX_HP = 50;  //プレイヤーの最大HP
	//SE
	int sound_handle_jump;  //ジャンプSE用
	int sound_handle_invincible;  //無敵SE用
	int sound_handle_damage;  //ダメージSE用
	int sound_handle_coin;  //コインSE用

};