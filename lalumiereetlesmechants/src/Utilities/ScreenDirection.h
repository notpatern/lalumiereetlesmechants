#pragma once
#include <cmath>
#include "Vector2.h"

namespace Utility
{
	inline constexpr float CELL_HEIGHT_IN_COLUMNS = 2.f;

	inline Vector2<float> ScreenDirection(const Vector2<float>& from, const Vector2<float>& to)
	{
		const float dx = to.x - from.x;
		const float dy = to.y - from.y;
		const float dxInRows = dx / CELL_HEIGHT_IN_COLUMNS;
		const float length = std::sqrt(dxInRows * dxInRows + dy * dy);

		if (length <= 0.f)
		{
			return { 0.f, 1.f };
		}

		return { dx / length, dy / length };
	}
}
