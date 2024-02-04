#pragma once
#include "../SceneBase.h"

class GameOverScene : public SceneBase
{
public:
	GameOverScene();
	~GameOverScene();

	virtual void Initialize() override;
	virtual ESceneType Update(float delta_seconds) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	int graph_handle_gameoverlogo;
	int graph_handle_gameovertext;
	Vector2D pos_gameoverlogo;
	Vector2D pos_gameovertext;

	virtual ESceneType GetSceneType() const override { return ESceneType::GAMEOVER_SCENE; }

};
