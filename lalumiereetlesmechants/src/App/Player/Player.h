#pragma once
#include "../../Utilities/Pool.h"
#include "../../Utilities/Vector2.h"
#include "../../Utilities/Countdown.h"
#include "../Manager/InputManager.h"
#include "../Render/Sprite.h"
#include "../Entites/Missile.h"
#include "../Gameplay/Collision.h"
#include "../Gameplay/Health.h"
#include "../Gameplay/IDamageable.h"


class Player : public IDamageable
{
public:
	explicit Player(const Utility::Vector2<float>& startPosition, Utility::Pool<Missile>& missiles);

	void Update(float dt, const InputManager& input, const Collision::CellRect& bounds);

	Utility::Vector2<float> getPosition() const { return m_position; }
	Sprite* getSprite() const { return m_sprite; }

	void TakeDamage(const DamageInfos& damage) override;
	bool IsDead() const override { return m_health.IsDead(); }
	int getHealth() const {return m_health.getCurrent(); }

	DamageInfos getContactDamage() const;

	bool getIsActive() const { return !IsDead(); }
	Collision::CellRect getHitboxRect() const { return m_hitbox.At(m_position); }
	bool IsInvincible() const { return !m_invincibility.IsFinished(); }

private:
	Sprite* m_sprite = nullptr;
	Utility::Vector2<float> m_position;
	Utility::Vector2<float> m_speed{ 30.f, 15.f };

	static constexpr float MISSILE_SPEED = 45.f;
	static constexpr int MISSILE_DAMAGE = 1;

	Utility::Pool<Missile>* m_missilePool;
	int m_maxMissiles{100};
	int m_remainingMissiles{m_maxMissiles};

	Utility::Countdown m_fireCooldown;
	float m_fireDelay{0.25f};

	static constexpr int MAX_HEALTH = 3;
	static constexpr float HIT_INVINCIBILITY_DURATION = 1.5f;

	static constexpr int HITBOX_WIDTH = 1;
	static constexpr int HITBOX_HEIGHT = 1;

	static constexpr int CONTACT_DAMAGE = 1000;

	Health m_health;
	Collision::Hitbox m_hitbox;
	Utility::Countdown m_invincibility;

	static Utility::Vector2<int> ReadDirection(const InputManager& input);
	void Move(float dt, const Utility::Vector2<int>& direction, const Collision::CellRect& bounds);
	void TryMoveTo(const Utility::Vector2<float>& target, const Collision::CellRect& bounds);

	static bool ReadShoot(const InputManager& input);
	void TryShoot();
};
