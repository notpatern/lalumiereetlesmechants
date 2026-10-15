#pragma once
#include "../../Utilities/Vector2.h"

enum class MovementType
{
	Straight,
	Sine
};

struct MovementSettings
{
	MovementType type = MovementType::Straight;
	float speed = 0.f;
	float amplitude = 0.f;
	float frequency = 0.f;
};

class Movement
{
public:
	void Start(const MovementSettings& settings, const Utility::Vector2<float>& start, const Utility::Vector2<float>& velocity, const Utility::Vector2<float>& target);
	[[nodiscard]] Utility::Vector2<float> Update(float deltaTime);

private:
	MovementSettings m_settings;
	Utility::Vector2<float> m_basePosition;
	Utility::Vector2<float> m_velocity;
	float m_elapsed = 0.f;
};
