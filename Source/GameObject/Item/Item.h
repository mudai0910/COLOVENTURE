#pragma once
#include "../GameObject.h"

class Item : public GameObject
{
public:
	Item();
	virtual ~Item();

	virtual void Initialize() override;
	virtual void Update(float delta_seconds) override;
	virtual void Draw(const Vector2D& screen_offset) override;
	virtual void Finalize() override;

	virtual void OnCollision(GameObject* object) override {}

protected:
	virtual void OnDestroy() override;
	int graphics_handle;

};