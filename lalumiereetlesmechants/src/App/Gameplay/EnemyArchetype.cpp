#include "EnemyArchetype.h"

namespace EnemyArchetypes
{
	const EnemyArchetype& Get(EnemyType type)
	{
		static const EnemyArchetype HUNTER
		{
			.maxHealth = 2,
			.hitboxSize = { 3, 2 },
			.contactDamage = 1,
			.points = 150,
			.movement = { .type = MovementType::Straight, .speed = 8.f },
			.weapon = { .fireInterval = 1.5f, .missileSpeed = 14.f, .missileDamage = 1, .aim = AimMode::AtPlayer },
			.sprite = SpriteUtils::SpriteType::Hunter,
			.hitSprite = SpriteUtils::SpriteType::HunterHit,
		};

		static const EnemyArchetype TURRET
		{
			.maxHealth = 5,
			.hitboxSize = { 5, 2 },
			.contactDamage = 1,
			.points = 250,
			.movement = { .type = MovementType::Sine, .amplitude = 1.5f, .frequency = 2.f },
			.weapon = { .fireInterval = 1.2f, .missileSpeed = 6.f, .missileDamage = 1, .missileSize = { 2, 2 } },
			.sprite = SpriteUtils::SpriteType::Turret,
			.hitSprite = SpriteUtils::SpriteType::TurretHit,
		};

		switch (type)
		{
		case EnemyType::Turret:
			return TURRET;
		case EnemyType::Hunter:
		default:
			return HUNTER;
		}
	}
}
