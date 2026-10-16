#include "Movement.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

#include "PlayArea.h"
#include "../../Utilities/ScreenDirection.h"

namespace
{
	float DistanceBeforeEdge(float position, float direction, float maxPosition)
	{
		if (direction > 0.f)
		{
			return (maxPosition - position) / direction;
		}
		if (direction < 0.f)
		{
			return -position / direction;
		}
		return std::numeric_limits<float>::infinity();
	}
}

void Movement::Start(const MovementSettings& settings, const Utility::Vector2<float>& start, const Utility::Vector2<float>& velocity, const Utility::Vector2<float>& target, const Utility::Vector2<int>& bodySize)
{
	m_settings = settings;
	m_basePosition = start;
	m_velocity = velocity;
	m_target = target;
	m_centerOffset = { bodySize.x / 2.f, bodySize.y / 2.f };
	m_elapsed = 0.f;
	m_maxX = static_cast<float>(PlayArea::WIDTH - bodySize.x);
	m_maxY = static_cast<float>(PlayArea::HEIGHT - bodySize.y);
	m_chargePhase = ChargePhase::Entering;

	if (settings.speed > 0.f)
	{
		const Utility::Vector2<float> direction = Utility::ScreenDirection(start, target);
		m_velocity = { direction.x * settings.speed, direction.y * settings.speed };
	}
}

Utility::Vector2<float> Movement::Update(float deltaTime, const Utility::Vector2<float>& playerCenter)
{
	m_elapsed += deltaTime;
	m_chargeJustEnded = false;

	switch (m_settings.type)
	{
	case MovementType::Charge:
		UpdateCharge(deltaTime, playerCenter);
		break;

	case MovementType::Seek:
		UpdateSeek(deltaTime, playerCenter);
		break;

	default:
		break;
	}

	m_basePosition.x += m_velocity.x * deltaTime;
	m_basePosition.y += m_velocity.y * deltaTime;

	if (m_settings.bounceOnEdges)
	{
		BounceOnEdges();
	}

	if (m_settings.stayOnScreen)
	{
		m_basePosition = ClampInPlayArea(m_basePosition, STAY_ON_SCREEN_MARGIN);
	}

	if (m_settings.type == MovementType::Sine)
	{
		return { m_basePosition.x, m_basePosition.y + m_settings.amplitude * std::sin(m_elapsed * m_settings.frequency) };
	}

	return m_basePosition;
}

void Movement::Nudge(const Utility::Vector2<float>& offset)
{
	m_basePosition.x += offset.x;
	m_basePosition.y += offset.y;
}

void Movement::BounceOnEdges()
{
	const bool passedRightEdge = m_velocity.x > 0.f && m_basePosition.x > m_maxX;
	const bool passedLeftEdge = m_velocity.x < 0.f && m_basePosition.x < 0.f;

	if (passedRightEdge || passedLeftEdge)
	{
		m_velocity.x = -m_velocity.x;
		m_basePosition.x = std::clamp(m_basePosition.x, 0.f, m_maxX);
	}
}

void Movement::UpdateCharge(float deltaTime, const Utility::Vector2<float>& playerCenter)
{
	m_chargeTimer.Update(deltaTime);

	switch (m_chargePhase)
	{
	case ChargePhase::Entering:
		if (BrakeToTarget(m_settings.speed, deltaTime))
		{
			StartTelegraph(playerCenter);
		}
		break;

	case ChargePhase::Telegraphing:
		if (m_chargeTimer.IsFinished())
		{
			m_chargePhase = ChargePhase::Charging;
		}
		break;

	case ChargePhase::Charging:
		if (BrakeToTarget(m_settings.chargeSpeed, deltaTime))
		{
			m_chargeJustEnded = true;
			StartPhase(ChargePhase::Pausing, m_settings.pauseBetweenCharges);
		}
		break;

	case ChargePhase::Pausing:
		if (m_chargeTimer.IsFinished())
		{
			StartTelegraph(playerCenter);
		}
		break;
	}
}

void Movement::StartPhase(ChargePhase phase, float duration)
{
	m_chargePhase = phase;
	m_chargeTimer.Start(duration);
}

void Movement::StartTelegraph(const Utility::Vector2<float>& playerCenter)
{
	const Utility::Vector2<float> aim = ClampInPlayArea({ playerCenter.x - m_centerOffset.x, playerCenter.y - m_centerOffset.y });
	const Utility::Vector2<float> direction = Utility::ScreenDirection(m_basePosition, aim);
	const float distanceToAim = std::sqrt(Utility::ScreenDistanceSquared(m_basePosition, aim));
	const float distance = std::min({ distanceToAim + m_settings.chargeOvershoot, DistanceBeforeEdge(m_basePosition.x, direction.x, m_maxX), DistanceBeforeEdge(m_basePosition.y, direction.y, m_maxY) });

	m_target = ClampInPlayArea({ m_basePosition.x + direction.x * distance, m_basePosition.y + direction.y * distance });
	StartPhase(ChargePhase::Telegraphing, m_settings.telegraphDuration);
}

bool Movement::BrakeToTarget(float maxSpeed, float deltaTime)
{
	const float distance = std::sqrt(Utility::ScreenDistanceSquared(m_basePosition, m_target));
	const float brakingSpeed = std::sqrt(2.f * m_settings.deceleration * distance);
	const float speed = std::min(maxSpeed, brakingSpeed);

	if (speed * deltaTime >= distance)
	{
		m_basePosition = m_target;
		m_velocity = { 0.f, 0.f };
		return true;
	}

	const Utility::Vector2<float> direction = Utility::ScreenDirection(m_basePosition, m_target);
	m_velocity = { direction.x * speed, direction.y * speed };
	return false;
}

void Movement::UpdateSeek(float deltaTime, const Utility::Vector2<float>& playerCenter)
{
	const Utility::Vector2<float> aim = ClampInPlayArea(playerCenter);
	const float distance = std::sqrt(Utility::ScreenDistanceSquared(m_basePosition, aim));
	if (distance <= 0.f)
	{
		return;
	}

	const float maxTurn = m_settings.turnRate * deltaTime;
	const float angle = Utility::ScreenAngle(m_velocity);
	const float wantedAngle = Utility::ScreenAngle(Utility::ScreenDirection(m_basePosition, aim));
	const float turn = std::remainder(wantedAngle - angle, 2.f * std::numbers::pi_v<float>);

	m_velocity = Utility::ScreenVector(angle + std::clamp(turn, -maxTurn, maxTurn), m_settings.speed);
}

Utility::Vector2<float> Movement::ClampInPlayArea(const Utility::Vector2<float>& position, float margin) const
{
	return { std::clamp(position.x, -margin, m_maxX + margin), std::clamp(position.y, -margin, m_maxY + margin) };
}
