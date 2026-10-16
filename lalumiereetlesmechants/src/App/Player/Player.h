#pragma once
#include "../../Utilities/Pool.h"
#include "../../Utilities/Vector2.h"
#include "../../Utilities/Countdown.h"
#include "../Manager/InputManager.h"
#include "../Entites/Missile.h"
#include "../Gameplay/Collision.h"
#include "../Gameplay/Health.h"
#include "../Gameplay/IDamageable.h"


class Player : public IDamageable, public Entity
{
public:
	explicit Player(const Utility::Vector2<float>& startPosition, Utility::Pool<Missile>& missiles);

	void Update(float dt, const InputManager& input, const Collision::CellRect& bounds);

	DamageResult TakeDamage(const DamageInfos& damage) override;
	bool IsDead() const override { return m_health.IsDead(); }
	int getHealth() const { return m_health.getCurrent(); }

	DamageInfos getContactDamage() const;

	bool getIsActive() const { return !IsDead(); }
	Collision::CellRect getHitboxRect() const { return m_hitbox.At(m_position); }
	Utility::Vector2<float> getAimPoint() const { return getHitboxRect().getCenter(); }
	bool IsInvincible() const { return !m_invincibility.IsFinished(); }
	bool IsVisible() const;

	Team getTeam() const { return Team::Player; }

private:
	Utility::Vector2<float> m_speed{30.f, 15.f};
	static constexpr float DIAGONAL_FACTOR = 0.7071f;

	static constexpr float MISSILE_SPEED = 45.f;
	static constexpr int MISSILE_DAMAGE = 1;

	Utility::Pool<Missile>* m_missilePool;

	Utility::Countdown m_fireCooldown;
	float m_fireDelay{0.25f};

	static constexpr int MAX_HEALTH = 3;
	static constexpr float HIT_INVINCIBILITY_DURATION = 1.5f;
	static constexpr float BLINK_PERIOD = 0.1f;

	static constexpr int BODY_WIDTH = 3;
	static constexpr int BODY_HEIGHT = 2;

	static constexpr int HITBOX_OFFSET_X = 1;
	static constexpr int HITBOX_OFFSET_Y = 1;
	static constexpr int HITBOX_WIDTH = 1;
	static constexpr int HITBOX_HEIGHT = 1;

	static constexpr int CONTACT_DAMAGE = 1000;

	Health m_health;
	Collision::Hitbox m_hitbox;
	Collision::Hitbox m_body;
	Utility::Countdown m_invincibility;

	static Utility::Vector2<int> ReadDirection(const InputManager& input);
	void Move(float dt, const Utility::Vector2<int>& direction, const Collision::CellRect& bounds);
	void TryMoveTo(const Utility::Vector2<float>& target, const Collision::CellRect& bounds);

	static bool ReadShoot(const InputManager& input);
	void TryShoot();
	void Die();
};
