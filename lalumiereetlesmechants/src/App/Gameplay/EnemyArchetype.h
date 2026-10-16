#pragma once
#include "ImpactFeedback.h"
#include "Movement.h"
#include "Weapon.h"
#include "../Render/SpriteType.h"
#include "../../Utilities/Vector2.h"

enum class EnemyType
{
	Hunter,
	Turret,
	Charger,
	Swarm
};

struct EnemyArchetype
{
	int maxHealth = 1;
	Utility::Vector2<int> hitboxSize{ 3, 2 };
	int contactDamage = 1;
	int points = 100;
	int xp = 1;
	MovementSettings movement;
	WeaponSettings weapon;
	SpriteUtils::SpriteType sprite = SpriteUtils::SpriteType::Hunter;
	SpriteUtils::SpriteType hitSprite = SpriteUtils::SpriteType::HunterHit;
	SpriteUtils::SpriteType telegraphSprite = SpriteUtils::SpriteType::NONE;
	ImpactFeedback killFeedback{ .shakeX = 1, .shakeDuration = 0.1f };
	ImpactFeedback chargeEndFeedback;
};

namespace EnemyArchetypes
{
	const EnemyArchetype& Get(EnemyType inType);
}
