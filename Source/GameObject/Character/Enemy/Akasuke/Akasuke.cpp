#include "Akasuke.h"
#include <DxLib.h>
#include <cmath>
#include "../../../../Manager/GraphicsManager/GraphicsManager.h"
#include "../../Player/Player.h"

Akasuke::Akasuke()
	: Enemy()
{
	attack_elapsed_time=0;
	is_reach_deadend = false;
	is_seek_player = false;
}

Akasuke::~Akasuke()
{
}

void Akasuke::Initialize()
{
	__super::Initialize();

	//視野範囲をコリジョンにする
	box_collision = BOX_COLLISION;
	//左向き時の描画オフセット値
	graph_offset = GRAPH_OFFSET_LEFT;
	//左向き時のボディコリジョン
	box_collision_body = BOX_COLLISION_BODY_LEFT;


	graphics_handle_buf.resize((int)EAnimationState::DIED + 1);

	graphics_handle_buf[(int)EAnimationState::IDLE].resize(ANIM_IDLE_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_rd_wait.bmp"),
		ANIM_IDLE_TOTAL_FRAME_NUM, ANIM_IDLE_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::IDLE]);

	graphics_handle_buf[(int)EAnimationState::WALK].resize(ANIM_WALK_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_rd_walk.bmp"),
		ANIM_WALK_TOTAL_FRAME_NUM, ANIM_WALK_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::WALK]);

	graphics_handle_buf[(int)EAnimationState::DAMAGE].resize(ANIM_DAMAGE_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_rd_damage.bmp"),
		ANIM_DAMAGE_TOTAL_FRAME_NUM, ANIM_DAMAGE_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::DAMAGE]);

	graphics_handle_buf[(int)EAnimationState::ATTACK].resize(ANIM_ATTACK_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_rd_attack.bmp"),
		ANIM_ATTACK_TOTAL_FRAME_NUM, ANIM_ATTACK_TOTAL_FRAME_NUM, 1, 56, 48,
		graphics_handle_buf[(int)EAnimationState::ATTACK]);

	graphics_handle_buf[(int)EAnimationState::DIED].resize(ANIM_DIED_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/enemy_death.bmp"),
		ANIM_DIED_TOTAL_FRAME_NUM, ANIM_DIED_TOTAL_FRAME_NUM, 1, 40, 40,
		graphics_handle_buf[(int)EAnimationState::DIED]);

	ShiftAnimationState(EAnimationState::WALK);
	ShiftMovementState(EMovementState::WALKING);
}

void Akasuke::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
	//プレイヤー探知フラグは常にfalseにしておく（コリジョン内にいるとtrueに上書きされる）
	is_seek_player = false;

}

//プレイヤーが視界に入った場合
void Akasuke::OnCollision(GameObject* object)
{
	if (object->GetObjectType() == EObjectType::PLAYER && GetAnimationState() != EAnimationState::DIED)
	{
		//Player* player = dynamic_cast<Player*>(object);

		//追いかけるフラグをtrue
		is_seek_player = true;

		// アタック範囲内
		if (!(is_turnright) && position.x + box_collision_body.x - BOX_COLLISION_BODY.x < object->GetPosition().x + object->GetBoxCollision().x
			|| (is_turnright) && object->GetPosition().x < position.x + box_collision_body.x + -BOX_COLLISION_BODY.x)
		{
			//アタック状態だったら
			if (GetAnimationState() == EAnimationState::ATTACK)
			{
				__super::OnCollision(object);
			}
			else
			{
				ShiftAnimationState(EAnimationState::ATTACK);
			}
		}
		//ボディコリジョン内
		if (!(is_turnright)&&position.x+ box_collision_body.x < object->GetPosition().x + object->GetBoxCollision().x
			|| (is_turnright)&& object->GetPosition().x<position.x + box_collision_body.x)
		{
			__super::OnCollision(object);
		}
	}

}

void Akasuke::MoveAuto()
{
	//プレイヤーを見つけたらスピードアップ
	walk_speed = is_seek_player ? 100 : 30;
	if (GetAnimationState() == EAnimationState::WALK)
	{
		__super::MoveAuto();
	}
}
//行き止まりに到達
void Akasuke::OnReachDeadEnd()
{
	if (is_reach_deadend==false) //視野範囲が行き止まりに到達したら
	{
		//左向きの場合positionをボディコリジョンがある座標にする
		if(!is_turnright)position.x += BOX_COLLISION_BODY_LEFT.x;

		//コリジョンの大きさをボディコリジョンにする
		box_collision = BOX_COLLISION_BODY;
		box_collision_body = BOX_COLLISION_BODY;
		graph_offset = GRAPH_OFFSET_RIGHT;

		//次にこの関数が呼ばれるのはボディコリジョンが到達したとき
		is_reach_deadend = true;
	}
	else  //ボディコリジョンが行き止まりに到達したら
	{
		//向きを反転させる
		is_turnright = !is_turnright;
		box_collision = BOX_COLLISION;
		//引き返す
		if (is_turnright) //左から右へ向かう場合
		{
			graph_offset = GRAPH_OFFSET_RIGHT;
			box_collision_body = BOX_COLLISION_BODY;
		}
		else  //右から左へ向かう場合
		{
			graph_offset = GRAPH_OFFSET_LEFT;
			box_collision_body = BOX_COLLISION_BODY_LEFT;
			position.x -=  box_collision_body.x;
		}

		is_reach_deadend = false;

	}
}

//リスポーン
void Akasuke::Respawn(Vector2D respawn_point)
{
	__super::Respawn(respawn_point);
	attack_elapsed_time = 0;
	is_reach_deadend = false;
	graph_offset = GRAPH_OFFSET_LEFT;
	box_collision_body = BOX_COLLISION_BODY_LEFT;
	box_collision = BOX_COLLISION;

}
