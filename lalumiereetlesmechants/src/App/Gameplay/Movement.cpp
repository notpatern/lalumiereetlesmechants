#include "Movement.h"
#include "PlayArea.h"
#include "../../Utilities/ScreenDirection.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

namespace
{
	float ComputeDistanceBeforeEdge(float inPosition, float inDirection, float inMaxPosition)
	{
		if (inDirection > 0.f)
		{
			return (inMaxPosition - inPosition) / inDirection;
		}
		if (inDirection < 0.f)
		{
			return -inPosition / inDirection;
		}
		return std::numeric_limits<float>::infinity();
	}
}

void Movement::Start(const MovementSettings& inSettings, const Utility::Vector2<float>& inStart, const Utility::Vector2<float>& inVelocity, const Utility::Vector2<float>& inTarget, const Utility::Vector2<int>& inBodySize)
{
	m_settings = inSettings;
	m_basePosition = inStart;
	m_velocity = inVelocity;
	m_target = inTarget;
	m_centerOffset = { inBodySize.x / 2.f, inBodySize.y / 2.f };
	m_elapsed = 0.f;
	m_maxX = static_cast<float>(PlayArea::WIDTH - inBodySize.x);
	m_maxY = static_cast<float>(PlayArea::HEIGHT - inBodySize.y);
	m_chargePhase = ChargePhase::Entering;

	if (inSettings.speed > 0.f)
	{
		const Utility::Vector2<float> direction = Utility::ScreenDirection(inStart, inTarget);
		m_velocity = { direction.x * inSettings.speed, direction.y * inSettings.speed };
	}
}

Utility::Vector2<float> Movement::Update(float inDeltaTime, const Utility::Vector2<float>& inPlayerCenter)
{
	m_elapsed += inDeltaTime;
	m_chargeJustEnded = false;

	switch (m_settings.type)
	{
	case MovementType::Charge:
		UpdateCharge(inDeltaTime, inPlayerCenter);
		break;

	case MovementType::Seek:
		UpdateSeek(inDeltaTime, inPlayerCenter);
		break;

	default:
		break;
	}

	m_basePosition.x += m_velocity.x * inDeltaTime;
	m_basePosition.y += m_velocity.y * inDeltaTime;

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

void Movement::Nudge(const Utility::Vector2<float>& inOffset)
{
	m_basePosition.x += inOffset.x;
	m_basePosition.y += inOffset.y;
}

void Movement::UpdateCharge(float inDeltaTime, const Utility::Vector2<float>& inPlayerCenter)
{
	m_chargeTimer.Update(inDeltaTime);

	switch (m_chargePhase)
	{
	case ChargePhase::Entering:
		if (BrakeToTarget(m_settings.speed, inDeltaTime))
		{
			StartTelegraph(inPlayerCenter);
		}
		break;

	case ChargePhase::Telegraphing:
		if (m_chargeTimer.IsFinished())
		{
			m_chargePhase = ChargePhase::Charging;
		}
		break;

	case ChargePhase::Charging:
		if (BrakeToTarget(m_settings.chargeSpeed, inDeltaTime))
		{
			m_chargeJustEnded = true;
			StartPhase(ChargePhase::Pausing, m_settings.pauseBetweenCharges);
		}
		break;

	case ChargePhase::Pausing:
		if (m_chargeTimer.IsFinished())
		{
			StartTelegraph(inPlayerCenter);
		}
		break;
	}
}

void Movement::StartPhase(ChargePhase inPhase, float inDuration)
{
	m_chargePhase = inPhase;
	m_chargeTimer.Start(inDuration);
}

void Movement::StartTelegraph(const Utility::Vector2<float>& inPlayerCenter)
{
	const Utility::Vector2<float> aim = ClampInPlayArea({ inPlayerCenter.x - m_centerOffset.x, inPlayerCenter.y - m_centerOffset.y });
	const Utility::Vector2<float> direction = Utility::ScreenDirection(m_basePosition, aim);
	const float distanceToAim = std::sqrt(Utility::ScreenDistanceSquared(m_basePosition, aim));
	const float distance = std::min({ distanceToAim + m_settings.chargeOvershoot, ComputeDistanceBeforeEdge(m_basePosition.x, direction.x, m_maxX), ComputeDistanceBeforeEdge(m_basePosition.y, direction.y, m_maxY) });

	m_target = ClampInPlayArea({ m_basePosition.x + direction.x * distance, m_basePosition.y + direction.y * distance });
	StartPhase(ChargePhase::Telegraphing, m_settings.telegraphDuration);
}

bool Movement::BrakeToTarget(float inMaxSpeed, float inDeltaTime)
{
	const float distance = std::sqrt(Utility::ScreenDistanceSquared(m_basePosition, m_target));
	const float brakingSpeed = std::sqrt(2.f * m_settings.deceleration * distance);
	const float speed = std::min(inMaxSpeed, brakingSpeed);

	if (speed * inDeltaTime >= distance)
	{
		m_basePosition = m_target;
		m_velocity = { 0.f, 0.f };
		return true;
	}

	const Utility::Vector2<float> direction = Utility::ScreenDirection(m_basePosition, m_target);
	m_velocity = { direction.x * speed, direction.y * speed };
	return false;
}

void Movement::UpdateSeek(float inDeltaTime, const Utility::Vector2<float>& inPlayerCenter)
{
	const Utility::Vector2<float> aim = ClampInPlayArea(inPlayerCenter);
	const float distance = std::sqrt(Utility::ScreenDistanceSquared(m_basePosition, aim));
	if (distance <= 0.f)
	{
		return;
	}

	const float maxTurn = m_settings.turnRate * inDeltaTime;
	const float angle = Utility::ScreenAngle(m_velocity);
	const float wantedAngle = Utility::ScreenAngle(Utility::ScreenDirection(m_basePosition, aim));
	const float turn = std::remainder(wantedAngle - angle, 2.f * std::numbers::pi_v<float>);

	m_velocity = Utility::ScreenVector(angle + std::clamp(turn, -maxTurn, maxTurn), m_settings.speed);
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

Utility::Vector2<float> Movement::ClampInPlayArea(const Utility::Vector2<float>& inPosition, float inMargin) const
{
	return { std::clamp(inPosition.x, -inMargin, m_maxX + inMargin), std::clamp(inPosition.y, -inMargin, m_maxY + inMargin) };
}
