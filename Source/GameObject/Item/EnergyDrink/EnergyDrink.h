#pragma once

#include "../Item.h"

class EnergyDrink : public Item
{
public:
	EnergyDrink();
	virtual ~EnergyDrink();

	virtual void Initialize() override;
	virtual void Draw(const Vector2D& screen_offset) override;

	virtual void OnCollision(GameObject* object) override;

protected:
	//virtual void OnDestroy() override;

};
