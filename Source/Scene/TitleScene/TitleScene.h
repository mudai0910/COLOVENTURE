#pragma once

#include "../SceneBase.h"

class Stage;

class TitleScene : public SceneBase
{
	enum class EButtonType : unsigned char
	{
		START,
		RULE,

		MAX,
	};

public:
	TitleScene();
	~TitleScene();

	virtual void Initialize() override;
	virtual ESceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	virtual ESceneType GetSceneType() const override { return ESceneType::TITLE_SCENE; }

private:
	// 現在読み込んでいるレベルデータ
	Stage* stage_data;
	//int graphics_handle;

	int sound_handle_button;  //ボタンSE用
	//グラフィックハンドル
	int graph_handle_background_distant;
	int graph_handle_background_close;
	int graph_handle_title;
	int graph_handle_cursor;
	int graph_handle_rulewindow;
	int graph_handle_startbutton_n;	//非選択時
	int graph_handle_startbutton_s;	//選択時
	int graph_handle_rulebutton_n;
	int graph_handle_rulebutton_s;
	int button_id;

	Vector2D pos_title;
	Vector2D pos_cursor;
	Vector2D pos_rulewindow;
	std::vector<Vector2D> button_pos;
	bool is_display_rule;	//ルールが表示されているか
};
