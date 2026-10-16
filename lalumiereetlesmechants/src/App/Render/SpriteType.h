#pragma once

namespace SpriteUtils
{
	enum class SpriteType
	{
		Hunter,
		HunterHit,
		Turret,
		TurretHit,
		Player,
		PlayerMissile,
		EnemyMissile,
		BigEnemyMissile,
		XpOrb,
		Charger,
		ChargerHit,
		ChargerTelegraph,
		Swarm,
		Spark,
		Smoke,
		LevelUpSpark,
		NONE
	};
	// nous permet de donner un flag à un sprite pour que le renderer
	// sache quelle texture (préloadée) render pour le current sprite
}
