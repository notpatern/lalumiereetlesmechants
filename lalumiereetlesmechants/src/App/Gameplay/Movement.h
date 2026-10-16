#pragma once
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"

enum class MovementType
{
	Straight,
	Sine,
	Charge
};

struct MovementSettings
{
	MovementType type = MovementType::Straight;
	float speed = 0.f;
	float amplitude = 0.f;
	float frequency = 0.f;
	bool bounceOnEdges = false;
	float deceleration = 0.f;
	float telegraphDuration = 0.f;
	float chargeSpeed = 0.f;
	float chargeOvershoot = 0.f;
	float pauseBetweenCharges = 0.f;
};

class Movement
{
public:
	void Start(const MovementSettings& settings, const Utility::Vector2<float>& start, const Utility::Vector2<float>& velocity, const Utility::Vector2<float>& target, const Utility::Vector2<int>& bodySize);
	[[nodiscard]] Utility::Vector2<float> Update(float deltaTime, const Utility::Vector2<float>& playerCenter);

	bool IsTelegraphing() const { return m_chargePhase == ChargePhase::Telegraphing; }

private:
	enum class ChargePhase
	{
		Entering,
		Telegraphing,
		Charging,
		Pausing
	};

	MovementSettings m_settings;
	Utility::Vector2<float> m_basePosition;
	Utility::Vector2<float> m_velocity;
	Utility::Vector2<float> m_target;
	Utility::Vector2<float> m_centerOffset;
	float m_elapsed = 0.f;
	float m_maxX = 0.f;
	float m_maxY = 0.f;
	ChargePhase m_chargePhase = ChargePhase::Entering;
	Utility::Countdown m_chargeTimer;

	void BounceOnEdges();
	void UpdateCharge(float deltaTime, const Utility::Vector2<float>& playerCenter);
	void StartPhase(ChargePhase phase, float duration);
	void StartTelegraph(const Utility::Vector2<float>& playerCenter);
	[[nodiscard]] bool BrakeToTarget(float maxSpeed, float deltaTime);
	Utility::Vector2<float> ClampInPlayArea(const Utility::Vector2<float>& position) const;
};
