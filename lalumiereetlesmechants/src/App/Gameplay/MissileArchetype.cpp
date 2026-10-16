#include "MissileArchetype.h"

namespace MissileArchetypes
{
	const MissileArchetype& Get(MissileType type)
	{
		static const MissileArchetype PLAYER_SHOT
		{
			.team = Team::Player,
			.hitboxSize = { 1, 1 },
			.damage = 1,
			.speed = 45.f,
			.sprite = SpriteUtils::SpriteType::PlayerMissile,
		};

		static const MissileArchetype ENEMY_SHOT
		{
			.team = Team::Enemy,
			.hitboxSize = { 1, 1 },
			.damage = 1,
			.speed = 12.f,
			.sprite = SpriteUtils::SpriteType::EnemyMissile,
		};

		static const MissileArchetype BIG_ENEMY_SHOT
		{
			.team = Team::Enemy,
			.hitboxSize = { 2, 2 },
			.damage = 1,
			.speed = 10.f,
			.sprite = SpriteUtils::SpriteType::BigEnemyMissile,
		};

		switch (type)
		{
		case MissileType::EnemyShot:
			return ENEMY_SHOT;
		case MissileType::BigEnemyShot:
			return BIG_ENEMY_SHOT;
		case MissileType::PlayerShot:
		default:
			return PLAYER_SHOT;
		}
	}
}
