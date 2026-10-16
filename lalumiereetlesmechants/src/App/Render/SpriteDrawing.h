#pragma once
#include "SpriteType.h"
#include <string_view>
#include <vector>
#include <windows.h>

namespace SpriteUtils
{
	// Le dessin d'un type de sprite : ses lignes (espace = transparent) et sa couleur console
	struct SpriteDrawing
	{
		std::vector<std::string_view> lines;
		WORD color;
	};

	inline const SpriteDrawing& GetSpriteDrawing(SpriteType type)
	{
		static const std::vector<std::string_view> HUNTER_LINES = { "<o>", " V " };
		static const std::vector<std::string_view> TURRET_LINES = { "[=O=]", " \\_/ " };
		static const std::vector<std::string_view> CHARGER_LINES = { "/#\\", "\\V/" };

		static const SpriteDrawing PLAYER{ { " ^ ", "/A\\" }, 0x0B };
		static const SpriteDrawing HUNTER{ HUNTER_LINES, 0x0C };
		static const SpriteDrawing HUNTER_HIT{ HUNTER_LINES, 0x0F };
		static const SpriteDrawing TURRET{ TURRET_LINES, 0x04 };
		static const SpriteDrawing TURRET_HIT{ TURRET_LINES, 0x0F };
		static const SpriteDrawing CHARGER{ CHARGER_LINES, 0x05 };
		static const SpriteDrawing CHARGER_HIT{ CHARGER_LINES, 0x0F };
		static const SpriteDrawing CHARGER_TELEGRAPH{ CHARGER_LINES, 0xCF };
		static const SpriteDrawing SWARM{ { "x" }, 0x0A };
		static const SpriteDrawing PLAYER_MISSILE{ { "|" }, 0x0B };
		static const SpriteDrawing ENEMY_MISSILE{ { "o" }, 0x0D };
		static const SpriteDrawing BIG_ENEMY_MISSILE{ { "/\\", "\\/" }, 0x0D };
		static const SpriteDrawing XP_ORB{ { "+" }, 0x0E };
		static const SpriteDrawing SPARK{ { "*" }, 0x0F };
		static const SpriteDrawing SMOKE{ { "," }, 0x08 };
		static const SpriteDrawing LEVEL_UP_SPARK{ { "+" }, 0x0E };
		static const SpriteDrawing NOTHING{ {}, 0x0F };

		switch (type)
		{
		case SpriteType::Player:
			return PLAYER;
		case SpriteType::Hunter:
			return HUNTER;
		case SpriteType::HunterHit:
			return HUNTER_HIT;
		case SpriteType::Turret:
			return TURRET;
		case SpriteType::TurretHit:
			return TURRET_HIT;
		case SpriteType::Charger:
			return CHARGER;
		case SpriteType::ChargerHit:
			return CHARGER_HIT;
		case SpriteType::ChargerTelegraph:
			return CHARGER_TELEGRAPH;
		case SpriteType::Swarm:
			return SWARM;
		case SpriteType::PlayerMissile:
			return PLAYER_MISSILE;
		case SpriteType::EnemyMissile:
			return ENEMY_MISSILE;
		case SpriteType::BigEnemyMissile:
			return BIG_ENEMY_MISSILE;
		case SpriteType::XpOrb:
			return XP_ORB;
		case SpriteType::Spark:
			return SPARK;
		case SpriteType::Smoke:
			return SMOKE;
		case SpriteType::LevelUpSpark:
			return LEVEL_UP_SPARK;
		default:
			return NOTHING;
		}
	}
}
