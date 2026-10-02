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
	explicit Player(const Utility::Vector2<float>& startPosition, Pool<Missile>& missiles);

	void Update(float dt, const InputManager& input);

	Utility::Vector2<float> getPosition() const { return m_position; }
	Sprite* getSprite() const { return m_sprite; }

private:
	Sprite* m_sprite = nullptr;
	Utility::Vector2<float> m_position;
	Utility::Vector2<float> m_speed{ 30.f, 15.f };

	Pool<Missile>* m_missilePool;
	int m_MaxMissiles{10};
	int m_currentMissile{m_MaxMissiles};

	Utility::Countdown m_shotCountdown;
	float m_shootCooldown{1.0f};

	static Utility::Vector2<int> ReadDirection(const InputManager& input);
	void Move(float dt, const Utility::Vector2<int>& direction);

	static bool ReadShoot(const InputManager& input);
	void Shoot(bool shoot);
};
