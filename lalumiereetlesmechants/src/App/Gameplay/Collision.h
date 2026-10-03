#pragma once
#include "../../Utilities/Vector2.h"

namespace Collision
{
	Utility::Vector2<int> ToCell(const Utility::Vector2<float>& position);

	bool IsSameCell(const Utility::Vector2<float>& a, const Utility::Vector2<float>& b);
}
