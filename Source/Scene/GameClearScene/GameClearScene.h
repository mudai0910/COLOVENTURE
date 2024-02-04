#pragma once
#include "../SceneBase.h"

class GameClearScene : public SceneBase
{
public:
	GameClearScene();
	~GameClearScene();

	virtual void Initialize() override;
	virtual ESceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	int graph_handle_background;
	int graph_handle_gameclearlogo;
	int graph_handle_gamecleartext;
	Vector2D pos_gameoverlogo;
	Vector2D pos_gameovertext;

	virtual ESceneType GetSceneType() const override { return ESceneType::GAMECLEAR_SCENE; }

};
