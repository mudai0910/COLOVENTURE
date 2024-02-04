#pragma once
#include <string>
#include "../SceneBase.h"

class Player;
class Character;
class Stage;

class InGameScene : public SceneBase
{

	enum class EStageID : unsigned int
	{
		TITLE_STAGE,
		STAGE_1,
		STAGE_2,

		MAX
	};
	// ゲームプレイ中に遷移する状態を表す
	enum class EGamePlayState : unsigned char
	{
		PLAYING,	// プレイ中
		STAGECLEAR,	// クリア演出
		MISS,	    // ミス演出
		STAGESTART,      // スタート
	};

public:
	InGameScene();
	virtual	~InGameScene();

	virtual void Initialize() override;
	virtual ESceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	virtual ESceneType GetSceneType() const override { return ESceneType::INGAME_SCENE; }

private:
	//ステージをロード
	void BuildStageData(int stage_id);
	// オブエジェクト同士の衝突をチェック.
	void CheckObjectCollided();
	// 引数のオブジェクトが重なっているか判定
	bool OverlapObjects(const GameObject* obj1, const GameObject* obj2);
	// キャラクターと地形との衝突を判定.
	void HandleCollisionToTerrain(class Character* character);
	// キャラクターと地形のめり込みを解消する.
	void ResolveCollisionToTerrain(class Character* character, int tile_pos_x, int tile_pos_y);

	// タイル上面との衝突
	bool CollideTop(class Character* character, int tile_pos_x, int tile_pos_y);	//bool CollideTop(class GameObject* character, int tile_pos_x, int tile_pos_y);
	// タイル底面との衝突
	bool CollideBottom(class Character* character, int tile_pos_x, int tile_pos_y);	//bool CollideBottom(class GameObject* character, int tile_pos_x, int tile_pos_y);
	// タイル左面との衝突
	bool CollideLeft(class Character* character, int tile_pos_x, int tile_pos_y);	//bool CollideLeft(class GameObject* character, int tile_pos_x, int tile_pos_y);
	// タイル右面との衝突
	bool CollideRight(class Character* character, int tile_pos_x, int tile_pos_y);	//bool CollideRight(class GameObject* character, int tile_pos_x, int tile_pos_y);

	// カメラ座標の更新
	void UpdateCameraOffset();

	//ステージ特有のデータのロード（背景、BGM）
	void LoadStage(int stage_id);
	//各オブジェクトのスポーン位置
	std::vector<Vector2D> object_spawn_points;

	// プレイヤーは頻繁にゲームからアクセスされるので
	// 特別に保持しておく.
	Player* player;
	Vector2D player_respawn_point;

	// 現在読み込んでいるレベルデータ
	Stage* stage_data;

	//現在のプレイ状態
	EGamePlayState gameplay_state;

	// ステージが始まってからの経過時間.
	// 残り時間のカウントダウンに使用
	float elapsed_time;
	//ステージ切り替えまでの待機時間
	float stage_waiting_time;
	// UI関連
	int ui_handle; 
	int time;			// 残り時間
	int total_coin;     //コイン枚数
	int total_score;    //スコア
	int player_left;	// プレイヤー残機
	int current_stage;  //現在のステージ番号（EStageIDの番号に対応）
	int graph_handle_hpbar_base;  //HPバー（ベース）
	int graph_handle_hpbar_meter;  //HPバー（メーター）
	int graph_handle_hpbar_flame;  //HPバー（フレーム）
	int graph_handle_icon_coin;  //コインアイコン
	int graph_handle_icon_colon;  //コロンちゃんアイコン

	//objects中リスポーンするオブジェクトが始めるインデックス
	int respawn_object_index;  

	//サウンド関連
	int sound_handle_stagestart;  //ステージスタートSE用
	int sound_handle_stageclear;  //ステージクリアSE用
	int sound_handle_miss;        //ミスSE用
	int sound_handle_1up;       //1UPSE用
};