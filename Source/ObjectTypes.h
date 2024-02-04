#pragma once

//オブジェクトのタイプ
enum class EObjectType : unsigned char
{
	NONE,
	GROUND,
	PLAYER,
	ENEMY,
	COIN,
	ENERGYDRINK,
	GOALFLAG
};