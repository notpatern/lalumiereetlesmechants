#include "Collision.h"
#include <cmath>

namespace Collision
{
	CellRect Hitbox::At(const Utility::Vector2<float>& inPosition) const
	{
		const Utility::Vector2<int> cell = ToCell(inPosition);

		CellRect rect;
		rect.topLeftCell = { cell.x + offset.x, cell.y + offset.y };
		rect.bottomRightCell = { rect.topLeftCell.x + (size.x - 1), rect.topLeftCell.y + (size.y - 1) };
		return rect;
	}

	Utility::Vector2<int> ToCell(const Utility::Vector2<float>& inPosition)
	{
		return { static_cast<int>(std::floor(inPosition.x)), static_cast<int>(std::floor(inPosition.y)) };
	}

	// Algorithme de Bresenham : parcourt les cases entre le depart et l'arrivee
	// uniquement avec des entiers
	bool CrossesRect(const Utility::Vector2<float>& inFrom, const Utility::Vector2<float>& inTo, const CellRect& inTarget)
	{
		const Utility::Vector2<int> end = ToCell(inTo);
		Utility::Vector2<int> cell = ToCell(inFrom);

		const int distanceX = std::abs(end.x - cell.x);
		const int distanceY = std::abs(end.y - cell.y);
		const int stepX = (cell.x < end.x) ? 1 : -1;
		const int stepY = (cell.y < end.y) ? 1 : -1;

		int error = distanceX - distanceY;

		while (true)
		{
			if (inTarget.ContainsCell(cell))
			{
				return true;
			}
			if (cell == end)
			{
				return false;
			}

			const int doubledError = 2 * error;
			if (doubledError > -distanceY)
			{
				error -= distanceY;
				cell.x += stepX;
			}
			if (doubledError < distanceX)
			{
				error += distanceX;
				cell.y += stepY;
			}
		}
	}
}
