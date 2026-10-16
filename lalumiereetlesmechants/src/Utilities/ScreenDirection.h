#pragma once
#include "Vector2.h"
#include <cmath>

namespace Utility
{
	// une case de console est 2x plus haute que large, sans ca tout irait 2x trop vite en vertical
	inline constexpr float CELL_HEIGHT_IN_COLUMNS = 2.f;

	inline float ScreenDistanceSquared(const Vector2<float>& inFrom, const Vector2<float>& inTo)
	{
		const float deltaXInRows = (inTo.x - inFrom.x) / CELL_HEIGHT_IN_COLUMNS;
		const float deltaY = inTo.y - inFrom.y;
		return deltaXInRows * deltaXInRows + deltaY * deltaY;
	}

	inline Vector2<float> ScreenDirection(const Vector2<float>& inFrom, const Vector2<float>& inTo)
	{
		const float deltaX = inTo.x - inFrom.x;
		const float deltaY = inTo.y - inFrom.y;
		const float length = std::sqrt(ScreenDistanceSquared(inFrom, inTo));

		if (length <= 0.f)
		{
			return { 0.f, 1.f };
		}

		return { deltaX / length, deltaY / length };
	}

	inline float ScreenAngle(const Vector2<float>& inVector)
	{
		return std::atan2(inVector.y, inVector.x / CELL_HEIGHT_IN_COLUMNS);
	}

	inline Vector2<float> ScreenVector(float inAngle, float inLength)
	{
		return { std::cos(inAngle) * inLength * CELL_HEIGHT_IN_COLUMNS, std::sin(inAngle) * inLength };
	}
}
