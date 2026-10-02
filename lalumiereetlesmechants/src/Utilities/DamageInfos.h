#pragma once
#include "Vector2.h"

enum damageType
{
	basic,
	explosion
};

struct DamageInfos
{
	int amount;
	damageType type;
	void* instigator;
	Utility::Vector2<int> position;
};
