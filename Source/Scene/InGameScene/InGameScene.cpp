#include <DxLib.h>
#include <string>
#include <fstream>
#include <cassert>
#include "InGameScene.h"
#include "../../SystemTypes.h"
#include "../../GameObject/Character/Player/Player.h"
#include "../../GameObject/Character/Enemy/Murasuke/Murasuke.h"
#include "../../GameObject/Character/Enemy/Akasuke/Akasuke.h"
#include "../../GameObject/Item/Coin/Coin.h"
#include "../../GameObject/Item/EnergyDrink/EnergyDrink.h"
#include "../../GameObject/Gimmick/GoalFlag/GoalFlag.h"
#include "../../GameObject/Field/Ground/Ground.h"
#include "../../GameObject/Field/BackGround/BackGround.h"
#include "../../Stage/Stage.h"
#include "../../ObjectTypes.h"
#include "../../Manager/GraphicsManager/GraphicsManager.h"
#include "../../Manager/SoundManager/SoundManager.h"

InGameScene::InGameScene()
{
	stage_data = nullptr;
	player = nullptr;
	ui_handle = 0;
	current_stage = 1;
	BuildStageData(current_stage);
	time = 100;
	total_coin = 0;
	total_score = 0;
	player_left = 2;
	stage_waiting_time=0;
	sound_handle_stageclear = 0;
	sound_handle_miss = 0;
	sound_handle_1up = 0;
}
InGameScene::~InGameScene()
{
	Finalize();
}
void InGameScene::Initialize()
{
	// 親クラスのInitialize()
	__super::Initialize();

	player_respawn_point = Vector2D();
	//スポーン位置の初期化
	object_spawn_points.clear();
	//背景とBGMをロードする
	LoadStage(current_stage);

	// 地面オブジェクトを追加
	Ground* ground = CreateObject<Ground>(Vector2D(0.f, 0.f));
	//リスポーンする必要のあるゲームオブジェクトのインデックスの始まりを取得
	respawn_object_index = objects.size();

	//プレイヤーの開始地点
	Vector2D player_spawn_point;

	// タイルマップに基づいたゲームオブジェクトのスポーン
	for (int y = 0; y < stage_data->GetStageTileNumY(); ++y)
	{
		for (int x = 0; x < stage_data->GetStageTileNumX(); ++x)
		{
			if (stage_data->GetTile(x, y) == "P")
			{
				//オブジェクトのスポーン位置を保持
				player_spawn_point = Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE);
				// そのタイルは空のタイルとして登録しなおす
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "C")
			{
				object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<Coin>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "E")
			{
				object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<EnergyDrink>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "M")
			{
				object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<Murasuke>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "A")
			{
				object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<Akasuke>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
			else if (stage_data->GetTile(x, y) == "G")
			{
			   object_spawn_points.push_back(Vector2D(x * MAPCHIP_SIZE, y * MAPCHIP_SIZE));
				CreateObject<GoalFlag>(object_spawn_points.back());
				stage_data->SetTile(x, y, "0");
			}
		}
	}

	object_spawn_points.push_back(player_spawn_point);
	//プレイヤーをスポーン※描画が一番上に来るように最後にスポーンする
	player = CreateObject<Player>(object_spawn_points.back());

	//マップデータをセット
	ground->SetMapData(stage_data->GetRawStage());
	//コインとスコアをセット
	//player->SetCoinNum(total_coin);
	player->SetScore(total_score);
	ui_handle = CreateFontToHandle(NULL, 24, 2);
	elapsed_time = 0.0f;
	time = 100;

	//画像をロードする
	graph_handle_hpbar_base = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/hpbar_base.bmp"));
	graph_handle_hpbar_meter = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/hpbar_meter.bmp"));
	graph_handle_hpbar_flame = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/hpbar_flame.bmp"));
	graph_handle_icon_coin = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/mapchip_035.bmp"));
	graph_handle_icon_colon = GraphicsManager::GetInstance().LoadGraph(_T("Resources/Images/collon_icon.bmp"));

	//SEをロードする
	sound_handle_stageclear = SoundManager::GetInstance().LoadSound("Resources/Sound/stageclear.wav");
	sound_handle_miss = SoundManager::GetInstance().LoadSound("Resources/Sound/miss.wav");
	sound_handle_1up = SoundManager::GetInstance().LoadSound("Resources/Sound/1up.wav");

	gameplay_state = EGamePlayState::STAGESTART;
}

ESceneType InGameScene::Update(float delta_seconds)
{
	//プレイ状態に応じて分岐
	switch (gameplay_state)
	{
	case EGamePlayState::PLAYING:
		// 親クラスのUpdate()
		__super::Update(delta_seconds);

		// オブジェクト同士の重なりを判定.
		CheckObjectCollided();

		//プレイヤーがステージ外に行かないようにする(マップ情報を参照するためこのクラスに記述)
		if (player->GetPosition().x < 0.0f)
		{
			player->SetPosition(Vector2D(0.0f, player->GetPosition().y));
		}
		else if (player->GetPosition().x + player->GetBoxCollision().x > stage_data->GetStageTileNumX() * MAPCHIP_SIZE)
		{
			player->SetPosition(Vector2D(stage_data->GetStageTileNumX() * MAPCHIP_SIZE - player->GetBoxCollision().x, player->GetPosition().y));

		}
		// カメラ座標の更新
		UpdateCameraOffset();
		// 残り時間の更新
		elapsed_time += delta_seconds;
		if (elapsed_time >= 1.0f)
		{
			time = (time > 0) ? time - 1 : 0;
			elapsed_time = 0.0f;
		}
		//コイン100枚で1UP
		if (player->GetCoinNum() !=0&&(total_coin + player->GetCoinNum())% 100 == 0)
		{
			//SE
			PlaySoundMem(sound_handle_1up, DX_PLAYTYPE_BACK, TRUE);
			//1UP
			player_left++;
			//トータルコインの枚数を保持
			total_coin += player->GetCoinNum();
			//枚数初期化
			player->SetCoinNum(0);
		}
		//プレイヤーが死んだOR時間切れ
		if (player->is_player_died || time <= 0)
		{
			StopSoundMem(sound_handle);
			//ミスSE
			PlaySoundMem(sound_handle_miss, DX_PLAYTYPE_BACK, TRUE);
			//MISSに遷移
			gameplay_state = EGamePlayState::MISS;
			return GetSceneType();
		}
		//プレイヤーがゴール
		if (player->is_player_goal)
		{
			StopSoundMem(sound_handle);
			//ステージクリアSE
			PlaySoundMem(sound_handle_stageclear, DX_PLAYTYPE_BACK, TRUE);
			//STAGECLEARに遷移
			gameplay_state = EGamePlayState::STAGECLEAR;
			return GetSceneType();
		}

		return GetSceneType();
		break;

	case EGamePlayState::STAGECLEAR:
		//ステージクリアSEの再生が終わったら
		if (CheckSoundMem(sound_handle_stageclear) == 0)
		{
			//次のレベルがあれば
			if (current_stage + 1 < (int)EStageID::MAX)
			{
				//コインとスコアを保持
				total_coin += player->GetCoinNum();
				//コインの枚数の初期化
				player->SetCoinNum(0);
				total_score = player->GetScore();
				//現在のステージIDを更新
				++current_stage;
				//次のレベルへ
				BuildStageData(current_stage);
				gameplay_state = EGamePlayState::STAGESTART;
				return GetSceneType();
			}
			else  //ゲームクリアシーンに遷移
			{
				return ESceneType::GAMECLEAR_SCENE;
			}
		}
		return GetSceneType();
		break;

	case EGamePlayState::MISS:
		//ミスSEの再生が終わったら
		if (CheckSoundMem(sound_handle_miss) == 0)
		{
			//残機を減らす
			player_left--;
			if (player_left >= 0)
			{
				//地面を除く各オブジェクトのリスポーン処理
				for (int i = 0; i < object_spawn_points.size(); ++i)
				{
					objects[respawn_object_index + i]->Respawn(object_spawn_points[i]);
				}
				// 残り時間を元に戻す
				time = 100;
				gameplay_state = EGamePlayState::STAGESTART;
				return GetSceneType();
			}
			else
			{
				//残基0でゲームオーバー
				return ESceneType::GAMEOVER_SCENE;
			}
		}
		return GetSceneType();
		break;

	case EGamePlayState::STAGESTART:
		//少し経過したらステージ遷移
		stage_waiting_time += delta_seconds;
		if (stage_waiting_time < 1.5f)
		{
			return GetSceneType();
		}
		else
		{
			stage_waiting_time = 0.0f;
			//BGM再生
			PlaySoundMem(sound_handle, DX_PLAYTYPE_LOOP, TRUE);
			//プレイ画面へ
			gameplay_state = EGamePlayState::PLAYING;
			return GetSceneType();
		}
		break;
	}

}

void InGameScene::Draw()
{

	// 親クラスのDraw()
	__super::Draw();

	//UI
	//表示するコインの枚数
	int current_coin_num = player->GetCoinNum() + total_coin;
	std::string coin_text = std::to_string(current_coin_num);
	std::string score_text = "SCORE " + std::to_string(player->GetScore());
	std::string time_text = "TIME " + std::to_string(time);
	std::string stage_text = "STAGE " + std::to_string((int)current_stage);

	if (gameplay_state == EGamePlayState::STAGESTART)
	{
		DrawBox(0, 0, SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y, 0, true);
		//ステージ名を中央に表示
		int text_width = GetDrawStringWidthToHandle(stage_text.c_str(), stage_text.size(), ui_handle);
		DrawStringToHandle(SCREEN_RESOLUTION_X/2 - text_width * 0.5f, SCREEN_RESOLUTION_Y/2-50, stage_text.c_str(), GetColor(255, 255, 255), ui_handle);
		//残機の表示
		std::string left_text = "×" + std::to_string(player_left);
		text_width = GetDrawStringWidthToHandle(left_text.c_str(), left_text.size(), ui_handle);
		DrawGraph(SCREEN_RESOLUTION_X / 2 - text_width*1.5f, SCREEN_RESOLUTION_Y / 2, graph_handle_icon_colon, true);
		DrawStringToHandle(SCREEN_RESOLUTION_X / 2 + text_width * 0.5f, SCREEN_RESOLUTION_Y / 2+5, left_text.c_str(), GetColor(255, 255, 255), ui_handle);
	}
	//上部のUI
	const int FONT_HEIGHT = 32;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, SCREEN_RESOLUTION_X, FONT_HEIGHT * 1.5f, 0, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);

	DrawGraph(SCREEN_RESOLUTION_X * 0.05f, 5, graph_handle_hpbar_base, true);
	Vector2D healthbar_tl = Vector2D(SCREEN_RESOLUTION_X * 0.05f, 5);
	Vector2D healthbar_br = healthbar_tl + Vector2D(160 * player->GetHPRatio(), FONT_HEIGHT);
	DrawExtendGraphF(healthbar_tl.x, healthbar_tl.y, healthbar_br.x, healthbar_br.y, graph_handle_hpbar_meter, true);
	DrawGraph(SCREEN_RESOLUTION_X * 0.05f, 5, graph_handle_hpbar_flame, true);


	DrawGraph(SCREEN_RESOLUTION_X * 0.32f, 5, graph_handle_icon_coin, true);
	DrawStringToHandle(SCREEN_RESOLUTION_X * 0.4f, 10, coin_text.c_str(), GetColor(255, 255, 255), ui_handle);
	DrawStringToHandle(SCREEN_RESOLUTION_X * 0.55f,10, score_text.c_str(), GetColor(255, 255, 255), ui_handle);
	DrawStringToHandle(SCREEN_RESOLUTION_X * 0.8f, 10, time_text.c_str(), GetColor(255, 255, 255), ui_handle);
}

void InGameScene::BuildStageData(int stage_id)
{
	bool need_initialize = false;
	// すでに他のレベルデータを読み込み済みであれば.
	if (stage_data)
	{
		stage_data->Finalize();
		//newしたオブジェクトを破棄
		delete stage_data;
		stage_data = nullptr;

		need_initialize = true;
	}
	//ステージデータのパス
	std::string stage_path = "Resources/StageData/Stage_" + std::to_string(stage_id) + ".csv";
	//ステージをロード
	stage_data = new Stage(stage_path);
	//ステージ更新時に初期化
	if (need_initialize)
	{
		//アセットを
		Finalize();
		Initialize();
	}
}
//ステージ特有のデータをロードする
void InGameScene::LoadStage(int stage_id)
{
	BackGround* background0 = CreateObject<BackGround>(Vector2D(0.f, 0.f));
	BackGround* background1 = CreateObject<BackGround>(Vector2D(0.f, 0.f));

	switch ((EStageID)stage_id)
	{
	case EStageID::STAGE_1:
		sound_handle = SoundManager::GetInstance().LoadSound("Resources/Sound/oceanstage.wav");
		background0->LoadGraphics("Resources/Images/bg_1_0.png");
		background0->SetDistanceScale(100.f);
		background1->LoadGraphics("Resources/Images/bg_1_1.png");
		background1->SetDistanceScale(5.f);
		break;
	case EStageID::STAGE_2:
		sound_handle = SoundManager::GetInstance().LoadSound("Resources/Sound/foreststage.wav");
		background0->LoadGraphics("Resources/Images/bg_2_0.png");
		background0->SetDistanceScale(100.f);
		background1->LoadGraphics("Resources/Images/bg_2_1.png");
		background1->SetDistanceScale(5.f);
		break;
	default:
		break;
	}
}
//当たり判定のチェック
void InGameScene::CheckObjectCollided()
{
	// 矩形同士の衝突判定.
	for (int i = 0; i < objects.size(); ++i)
	{
		GameObject* obj1 = objects[i];

		//Characterクラスは地面との衝突判定をする
		if (obj1->GetObjectType() == EObjectType::PLAYER || obj1->GetObjectType() == EObjectType::ENEMY)
		{
			Character* character = dynamic_cast<Character*>(obj1);
			//地面との衝突判定
			HandleCollisionToTerrain(character);
		}
		//画面外にいるオブジェクトは当たり判定対象外
		if (obj1->GetPosition().x < screen_offset.x || screen_offset.x + SCREEN_RESOLUTION_X < (obj1->GetPosition().x))continue;

		Vector2D obj1_center = obj1->GetPosition() + obj1->GetBoxCollision() * 0.5f;
		for (int j = i + 1; j < objects.size(); ++j)
		{
			GameObject* obj2 = objects[j];
			Vector2D obj2_center = obj2->GetPosition() + obj2->GetBoxCollision() * 0.5f;
			//画面外にいるオブジェクトは当たり判定対象外
			if (obj2->GetPosition().x < screen_offset.x || screen_offset.x + SCREEN_RESOLUTION_X < (obj2->GetPosition().x))continue;

			// 衝突判定
			if (OverlapObjects(obj1, obj2))
			{
				//双方に通知
				obj1->OnCollision(obj2);
				obj2->OnCollision(obj1);
			}
		}
	}
}
//オブジェクト同士の重なりを判定
bool InGameScene::OverlapObjects(const GameObject* A, const GameObject* B)
{
	float A_top = A->GetPosition().y;
	float A_bottom = A->GetPosition().y + A->GetBoxCollision().y;
	float A_left = A->GetPosition().x;
	float A_right = A->GetPosition().x + A->GetBoxCollision().x;

	float B_top = B->GetPosition().y;
	float B_bottom = B->GetPosition().y + B->GetBoxCollision().y;
	float B_left = B->GetPosition().x;
	float B_right = B->GetPosition().x + B->GetBoxCollision().x;

	if (A_top > B_bottom		// Aの上面はBの底面よりも低い
		|| A_right < B_left		// Aの右面はBの左面よりも左にある
		|| A_bottom < B_top		// Aの底面はBの上面よりも高い
		|| A_left > B_right)	// Aの左面はBの右面よりも右にある
		return false;

	return true;
}
//地面とキャラクターのコリジョン判定
void InGameScene::HandleCollisionToTerrain(Character* character)
{
	assert(character != nullptr);

	std::string tile;

	int tile_pos_left_x = std::floor(character->GetPosition().x / MAPCHIP_SIZE);  //左側のx位置
	int tile_pos_right_x = std::floor((character->GetPosition().x + character->GetBoxCollision().x) / MAPCHIP_SIZE);  //右側のx位置
	int tile_pos_upper_y = std::floor(character->GetPosition().y / MAPCHIP_SIZE);  //上側のy位置
	int tile_pos_lower_y = std::floor((character->GetPosition().y + character->GetBoxCollision().y) / MAPCHIP_SIZE);  //下側のy位置

	// アクターの上面の衝突をテスト
	{
		tile = stage_data->GetTile(tile_pos_left_x, tile_pos_upper_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_left_x, tile_pos_upper_y);
		tile = stage_data->GetTile(tile_pos_right_x, tile_pos_upper_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_right_x, tile_pos_upper_y);
	}

	// アクターの底面の衝突をテスト
	{
		//プレイヤーは両足が離れたら落下し、敵は片足が離れたら方向転換する
		if(stage_data->GetTile(tile_pos_left_x, tile_pos_lower_y)=="0" && stage_data->GetTile(tile_pos_right_x, tile_pos_lower_y)=="0")
		{
			character->OnFallAllFoots();
		}
		else if (stage_data->GetTile(tile_pos_left_x, tile_pos_lower_y) == "0" || stage_data->GetTile(tile_pos_right_x, tile_pos_lower_y) == "0")
		{
			character->OnReachDeadEnd();
		}
		else
		{
			ResolveCollisionToTerrain(character, tile_pos_left_x, tile_pos_lower_y);
			ResolveCollisionToTerrain(character, tile_pos_right_x, tile_pos_lower_y);
		}
	}

	// アクターの左面の衝突をテスト
	{
		tile = stage_data->GetTile(tile_pos_left_x, tile_pos_upper_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_left_x, tile_pos_upper_y);
		tile = stage_data->GetTile(tile_pos_left_x, tile_pos_lower_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_left_x, tile_pos_lower_y);
	}

	// アクターの右面の衝突をテスト
	{
		tile = stage_data->GetTile(tile_pos_right_x, tile_pos_upper_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_right_x, tile_pos_upper_y);
		tile = stage_data->GetTile(tile_pos_right_x, tile_pos_lower_y);
		if (tile != "0") ResolveCollisionToTerrain(character, tile_pos_right_x, tile_pos_lower_y);
	}
}

//地面のコリジョン解決
void InGameScene::ResolveCollisionToTerrain(Character* character, int tile_pos_x, int tile_pos_y)
{
	std::string str = stage_data->GetTile(tile_pos_x, tile_pos_y);
	if (str.empty())
	{
		return;
	}
	int tile_type = std::stoi(str);
	// レベルデータに登録されている番号に応じて衝突判定する辺を変更する
	// tile_typeが10~16は足場。下から上の一方通行にする
	if (tile_type >= 10 && tile_type <= 16)
	{
		// キャラクターの底面と足場の上面のみの衝突判定
		CollideTop(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 1)
	{
		if (CollideLeft(character, tile_pos_x, tile_pos_y)) return;
		CollideTop(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 2)
	{
		CollideTop(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 3)
	{
		if (CollideRight(character, tile_pos_x, tile_pos_y)) return;
		CollideTop(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 4)
	{
		CollideLeft(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 5)
	{
		if (CollideTop(character, tile_pos_x, tile_pos_y)) return;
		if (CollideLeft(character, tile_pos_x, tile_pos_y)) return;
		if (CollideRight(character, tile_pos_x, tile_pos_y)) return;
		CollideBottom(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 6)
	{
		CollideRight(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 7)
	{
		if (CollideLeft(character, tile_pos_x, tile_pos_y)) return;
		CollideBottom(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 8)
	{
		CollideBottom(character, tile_pos_x, tile_pos_y);
		return;
	}
	else if (tile_type == 9)
	{
		if (CollideRight(character, tile_pos_x, tile_pos_y)) return;
		CollideBottom(character, tile_pos_x, tile_pos_y);
		return;
	}

	return;
}
bool InGameScene::CollideTop(Character* character, int tile_pos_x, int tile_pos_y)
{
	float top = tile_pos_y * MAPCHIP_SIZE;

	// 「衝突した瞬間」であることを知りたい.
	// 前フレームの位置が衝突範囲外であり、今フレームが衝突範囲内であれば「衝突した瞬間」だとわかる.
	// アクター底面はタイルの上面よりも低い位置にある
	// && 前フレームのアクター底面はタイルの上面よりも高い位置にいた
	if (character->GetPosition().y + character->GetBoxCollision().y > top
		&& character->GetPrevPosition().y + character->GetBoxCollision().y <= top)
	{
		Vector2D vel = character->GetVelocity();
		character->SetVelocity(Vector2D(vel.x, 0.0f));
		Vector2D pos = character->GetPosition();
		character->SetPosition(Vector2D(pos.x, top - character->GetBoxCollision().y ));
		Vector2D prev_pos = character->GetPrevPosition();
		character->SetPrevPosition(Vector2D(prev_pos.x, pos.y));
		// 何らかの地面・足場に立っているので「歩きモード」に移行.
		character->ShiftMovementState(EMovementState::WALKING);
		return true;
	}

	return false;
}

bool InGameScene::CollideBottom(Character* character, int tile_pos_x, int tile_pos_y)
{
	float bottom = tile_pos_y * MAPCHIP_SIZE + MAPCHIP_SIZE;

	// アクター上面はタイルの底面よりも高い位置にある
	// && 前フレームのアクター上面はタイルの底面よりも低い位置にいた
	if (character->GetPosition().y < bottom
		&& character->GetPrevPosition().y >= bottom)
	{
		Vector2D vel = character->GetVelocity();
		character->SetVelocity(Vector2D(vel.x, 0.0f));
		Vector2D pos = character->GetPosition();
		character->SetPosition(Vector2D(pos.x, bottom));
		Vector2D prev_pos = character->GetPrevPosition();
		character->SetPrevPosition(Vector2D(prev_pos.x, pos.y));

		return true;
	}
	return false;
}

bool InGameScene::CollideLeft(Character* character, int tile_pos_x, int tile_pos_y)
{
	float left = tile_pos_x * MAPCHIP_SIZE;

	// アクターの右面はタイル左面よりも右の位置にいる
	// && 前フレームのアクターの右面はタイル左面よりも左の位置にいた
	if (character->GetPosition().x + character->GetBoxCollision().x > left
		&& character->GetPrevPosition().x + character->GetBoxCollision().x <= left)
	{
		Vector2D vel = character->GetVelocity();
		character->SetVelocity(Vector2D(0.0f, vel.y));
		Vector2D pos = character->GetPosition();
		character->SetPosition(Vector2D(left - character->GetBoxCollision().x /*- 0.01f*/, pos.y));
		Vector2D prev_pos = character->GetPrevPosition();
		character->SetPrevPosition(Vector2D(pos.x, prev_pos.y));
		//壁にぶつかったことを通知
		character->OnReachDeadEnd();
		return true;
	}

	return false;
}

bool InGameScene::CollideRight(Character* character, int tile_pos_x, int tile_pos_y)
{
	float right = tile_pos_x * MAPCHIP_SIZE + MAPCHIP_SIZE;

	// アクター左面はタイル右面よりも左の位置にいる
	// && 前フレームのアクター左面はタイル右面よりも右の位置にいた
	if (character->GetPosition().x < right
		&& character->GetPrevPosition().x >= right)
	{
		Vector2D vel = character->GetVelocity();
		character->SetVelocity(Vector2D(0.0f, vel.y));
		Vector2D pos = character->GetPosition();
		character->SetPosition(Vector2D(right, pos.y));
		Vector2D prev_pos = character->GetPrevPosition();
		character->SetPrevPosition(Vector2D(pos.x, prev_pos.y));
		//壁にぶつかったことを通知
		character->OnReachDeadEnd();
		return true;
	}

	return false;
}
//カメラのスクロール
void InGameScene::UpdateCameraOffset()
{
	// スクリーン幅の40％を超えて右に移動するとカメラも移動する.
	//
	const float SCROLL_BORDER_RIGHT = SCREEN_RESOLUTION_X * 0.4f;
	const float SCROLL_BORDER_LEFT = SCREEN_RESOLUTION_X * 0.2f;
	//最大でスクロールできる右端の位置
	const float MAX_SCROLL_RIGHT = stage_data->GetStageTileNumX() * MAPCHIP_SIZE - SCREEN_RESOLUTION_X;
	//プレイヤーのスクリーン上の位置
	float diff = player->GetPosition().x - screen_offset.x;
	if (diff > SCROLL_BORDER_RIGHT)
	{
		screen_offset.x += diff - SCROLL_BORDER_RIGHT;
	}
	else if (diff < SCROLL_BORDER_LEFT)
	{
		screen_offset.x += diff - SCROLL_BORDER_LEFT;
	}

	if (screen_offset.x > MAX_SCROLL_RIGHT)
	{
		screen_offset.x = MAX_SCROLL_RIGHT;
	}
	else if (screen_offset.x < 0)
	{
		screen_offset.x = 0.0f;
	}
}

void InGameScene::Finalize()
{
	// 親クラスのFinalize()
	__super::Finalize();
}