#pragma once
#include "../../Utilities/Pool.h"
#include "../../Utilities/Vector2.h"
#include "../../Utilities/Countdown.h"
#include "../Manager/InputManager.h"
#include "../Entites/Entities.h"
#include "../Entites/Missile.h"
#include "../Gameplay/Collision.h"
#include "../Gameplay/DamageInfos.h"
#include "../Gameplay/Experience.h"
#include "../Gameplay/Health.h"
#include "../Gameplay/IDamageable.h"
#include "../Gameplay/MissileArchetype.h"
#include <array>

struct ShotSettings
{
	int missileCount = 1;
	float spacing = 0.f;
	float spreadSpeed = 0.f;
	float fireInterval = 0.25f;
	MissileType missile = MissileType::PlayerShot;
};

struct PlayerLevel
{
	ShotSettings shot;
	int livesRestored = 1;
};

class Player : public IDamageable, public Entity
{
public:
	explicit Player(const Utility::Vector2<float>& inStartPosition, Utility::Pool<Missile>& inMissiles);

	void Update(float inDeltaTime, const InputManager& inInput, const Collision::CellRect& inBounds);

	DamageResult TakeDamage(const DamageInfos& inDamage) override;
	bool IsDead() const override { return m_health.IsDead(); }

	void AddXp(int inAmount);

	int getHealth() const { return m_health.getCurrent(); }
	int getMaxHealth() const { return m_health.getMax(); }
	const Experience& getExperience() const { return m_experience; }
	DamageInfos getContactDamage() const;

	Collision::CellRect getHitboxRect() const { return m_hitbox.At(m_position); }
	Collision::CellRect getBodyRect() const { return m_body.At(m_position); }
	Utility::Vector2<float> getAimPoint() const { return getHitboxRect().getCenter(); }

	bool IsActive() const { return !IsDead(); }
	bool IsInvincible() const { return !m_invincibility.IsFinished(); }
	bool IsVisible() const;

private:
	static constexpr float SPEED_X = 24.f;
	static constexpr float SPEED_Y = 12.f;
	static constexpr float DIAGONAL_FACTOR = 0.7071f;

	static constexpr int BODY_WIDTH = 3;
	static constexpr int BODY_HEIGHT = 2;

	static constexpr int HITBOX_OFFSET_X = 1;
	static constexpr int HITBOX_OFFSET_Y = 1;
	static constexpr int HITBOX_WIDTH = 1;
	static constexpr int HITBOX_HEIGHT = 1;

	static constexpr int MAX_HEALTH = 3;
	static constexpr float HIT_INVINCIBILITY_DURATION = 0.8f;
	static constexpr float BLINK_PERIOD = 0.1f;
	static constexpr int CONTACT_DAMAGE = 1000;

	static constexpr float MISSILE_SPAWN_OFFSET_Y = 1.f;

	static constexpr std::array LEVELS
	{
		PlayerLevel{ .shot = { .missileCount = 1, .fireInterval = 0.25f }, .livesRestored = 0 },
		PlayerLevel{ .shot = { .missileCount = 2, .spacing = 2.f, .fireInterval = 0.25f } },
		PlayerLevel{ .shot = { .missileCount = 2, .spacing = 2.f, .fireInterval = 0.20f } },
		PlayerLevel{ .shot = { .missileCount = 3, .spreadSpeed = 25.f, .fireInterval = 0.20f } },
		PlayerLevel{ .shot = { .missileCount = 3, .spreadSpeed = 25.f, .fireInterval = 0.16f } },
		PlayerLevel{ .shot = { .missileCount = 5, .spreadSpeed = 25.f, .fireInterval = 0.16f } },
		PlayerLevel{ .shot = { .missileCount = 5, .spreadSpeed = 25.f, .fireInterval = 0.13f } },
		PlayerLevel{ .shot = { .missileCount = 5, .spreadSpeed = 25.f, .fireInterval = 0.10f } },
	};
	static_assert(LEVELS.size() == Experience::MAX_LEVEL, "Il faut une fiche par niveau d'experience");

	Utility::Pool<Missile>* m_missilePool;
	Utility::Countdown m_fireCooldown;

	Health m_health;
	Collision::Hitbox m_hitbox;
	Collision::Hitbox m_body;
	Utility::Countdown m_invincibility;
	Experience m_experience;

	static Utility::Vector2<int> ReadDirection(const InputManager& inInput);
	void Move(float inDeltaTime, const Utility::Vector2<int>& inDirection, const Collision::CellRect& inBounds);
	void TryMoveTo(const Utility::Vector2<float>& inTarget, const Collision::CellRect& inBounds);

	static bool ReadShoot(const InputManager& inInput);
	void TryShoot();

	void Die();
};
