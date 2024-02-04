#pragma once
#include "../Item.h"

class Coin : public Item
{
public:
	Coin();
	virtual ~Coin();

	virtual void Initialize() override;
	virtual void Draw(const Vector2D& screen_offset) override;

	virtual void OnCollision(GameObject* object) override;
};