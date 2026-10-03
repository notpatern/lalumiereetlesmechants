#pragma once
#include "../../Utilities/Pool.h"
#include "../../Utilities/Vector2.h"
#include "../../Utilities/Countdown.h"
#include "../Manager/InputManager.h"
#include "../Render/Sprite.h"
#include "../Entites//Missile.h"


class Player
{
public:
	explicit Player(const Utility::Vector2<float>& startPosition, Utility::Pool<Missile>& missiles);

	void Update(float dt, const InputManager& input);

	Utility::Vector2<float> getPosition() const { return m_position; }
	Sprite* getSprite() const { return m_sprite; }

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
	float m_fireDelay{1.0f};

	static Utility::Vector2<int> ReadDirection(const InputManager& input);
	void Move(float dt, const Utility::Vector2<int>& direction);

	static bool ReadShoot(const InputManager& input);
	void TryShoot();
};
