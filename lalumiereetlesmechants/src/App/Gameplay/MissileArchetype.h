#pragma once
#include "DamageInfos.h"
#include "../Render/SpriteType.h"
#include "../../Utilities/Vector2.h"

enum class MissileType
{
	PlayerShot,
	EnemyShot,
	BigEnemyShot
};

struct MissileArchetype
{
	Team team = Team::Player;
	Utility::Vector2<int> hitboxSize{ 1, 1 };
	int damage = 1;
	float speed = 20.f;
	SpriteUtils::SpriteType sprite = SpriteUtils::SpriteType::PlayerMissile;
};

namespace MissileArchetypes
{
	const MissileArchetype& Get(MissileType inType);
}
