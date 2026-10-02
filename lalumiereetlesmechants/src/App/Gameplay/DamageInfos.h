#pragma once
#include "../../Utilities/Vector2.h"

enum class Team
{
	Player,
	Enemy
};

enum class DamageType
{
	Basic,
	Explosion
};

struct DamageInfos
{
	int amount = 1;
	DamageType type = DamageType::Basic;
	Team source = Team::Player;
	Utility::Vector2<float> position;
};
