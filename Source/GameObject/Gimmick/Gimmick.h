#pragma once
#include "../GameObject.h"

class Gimmick : public GameObject
{
public:
	Gimmick();
	virtual ~Gimmick();

	virtual void Initialize() override;
};