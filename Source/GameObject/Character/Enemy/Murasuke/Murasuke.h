#pragma once

#include "../Enemy.h"

class Murasuke : public Enemy
{
public:
	Murasuke();
	virtual ~Murasuke();

	virtual void Initialize() override;
	
};
