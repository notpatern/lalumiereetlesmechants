#include "EnemyArchetype.h"

namespace EnemyArchetypes
{
	const EnemyArchetype& Get(EnemyType inType)
	{
		static const EnemyArchetype HUNTER
		{
			.maxHealth = 2,
			.hitboxSize = { 3, 2 },
			.contactDamage = 1,
			.points = 150,
			.xp = 1,
			.movement = { .type = MovementType::Straight, .speed = 3.f },
			.weapon = { .fireInterval = 2.f, .aim = AimMode::AtPlayer, .missile = MissileType::EnemyShot },
			.sprite = SpriteUtils::SpriteType::Hunter,
			.hitSprite = SpriteUtils::SpriteType::HunterHit,
		};

		static const EnemyArchetype TURRET
		{
			.maxHealth = 10,
			.hitboxSize = { 5, 2 },
			.contactDamage = 1,
			.points = 250,
			.xp = 3,
			.movement = { .type = MovementType::Sine, .amplitude = 1.5f, .frequency = 2.f, .bounceOnEdges = true },
			.weapon = { .fireInterval = 1.f, .missile = MissileType::BigEnemyShot },
			.sprite = SpriteUtils::SpriteType::Turret,
			.hitSprite = SpriteUtils::SpriteType::TurretHit,
			.killFeedback = { .hitStop = 0.08f, .shakeX = 2, .shakeY = 1, .shakeDuration = 0.25f },
		};

		static const EnemyArchetype CHARGER
		{
			.maxHealth = 10,
			.hitboxSize = { 3, 2 },
			.contactDamage = 1,
			.points = 150,
			.xp = 3,
			.movement = { .type = MovementType::Charge, .speed = 12.f, .deceleration = 60.f, .telegraphDuration = 0.8f, .chargeSpeed = 40.f, .chargeOvershoot = 2.5f, .pauseBetweenCharges = 0.8f },
			.sprite = SpriteUtils::SpriteType::Charger,
			.hitSprite = SpriteUtils::SpriteType::ChargerHit,
			.telegraphSprite = SpriteUtils::SpriteType::ChargerTelegraph,
			.killFeedback = { .hitStop = 0.08f, .shakeX = 2, .shakeY = 1, .shakeDuration = 0.25f },
			.chargeEndFeedback = { .shakeX = 1, .shakeDuration = 0.1f },
		};

		static const EnemyArchetype SWARM
		{
			.maxHealth = 1,
			.hitboxSize = { 1, 1 },
			.contactDamage = 1,
			.points = 50,
			.xp = 1,
			.movement = { .type = MovementType::Seek, .speed = 10.f, .separationRadius = 1.5f, .separationSpeed = 8.f, .stayOnScreen = true, .turnRate = 2.f },
			.sprite = SpriteUtils::SpriteType::Swarm,
			.hitSprite = SpriteUtils::SpriteType::Swarm,
		};

		switch (inType)
		{
		case EnemyType::Turret:
			return TURRET;
		case EnemyType::Charger:
			return CHARGER;
		case EnemyType::Swarm:
			return SWARM;
		case EnemyType::Hunter:
		default:
			return HUNTER;
		}
	}
}
