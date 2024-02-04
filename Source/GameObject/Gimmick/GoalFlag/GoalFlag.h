#pragma once

#include "../Gimmick.h"

class GoalFlag : public Gimmick
{
public:
	GoalFlag();
	virtual ~GoalFlag();

	virtual void Initialize();
	virtual void Draw(const Vector2D& screen_offset);

	virtual void OnCollision(GameObject* object);


private:
	int graphics_handle;
};


