#pragma once

#include "../Enemy.h"

class Player;

class Akasuke : public Enemy
{
public:
	Akasuke();
	virtual ~Akasuke();

	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	void OnCollision(GameObject* object) override;

protected:
	void MoveAuto() override;
	void  OnReachDeadEnd() override;
	void Respawn(Vector2D respawn_point) override;

private:
	// 攻撃開始時からの経過時間.
	float attack_elapsed_time;
	//ボディコリジョン※通常のコリジョンは視野判定に使用
	Vector2D box_collision_body;
	//コリジョンが行き止まりについたか
	bool is_reach_deadend;
	//プレイヤーを見つけたか
	bool is_seek_player;
	//進行方向でコリジョンとオフセット値を変化させる
	const Vector2D BOX_COLLISION= Vector2D(150.0f, 32.0f);
	const Vector2D GRAPH_OFFSET_LEFT= Vector2D(-120.0f, 15.0f);
	const Vector2D GRAPH_OFFSET_RIGHT= Vector2D(5.0f, 15.0f);
	const Vector2D BOX_COLLISION_BODY_LEFT= Vector2D(125.0f, 32.0f);
	const Vector2D BOX_COLLISION_BODY = Vector2D(25.0f, 32.0f);
};

