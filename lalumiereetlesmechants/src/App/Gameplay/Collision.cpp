#include "Collision.h"
#include <cmath>

namespace Collision
{
	Utility::Vector2<int> ToCell(const Utility::Vector2<float>& position)
	{
		return { static_cast<int>(std::floor(position.x)), static_cast<int>(std::floor(position.y))};
	}

	bool IsSameCell(const Utility::Vector2<float>& a, const Utility::Vector2<float>& b)
	{
		return (ToCell(a) == ToCell(b));
	}
}
