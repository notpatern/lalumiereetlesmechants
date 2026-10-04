#include "Collision.h"
#include <cmath>

namespace Collision
{
	CellRect Hitbox::At(const Utility::Vector2<float>& position) const
	{
		const Utility::Vector2<int> cell = ToCell(position);

		CellRect rect;
		rect.topLeftCell = {cell.x + offset.x, cell.y + offset.y};
		rect.bottomRightCell = {rect.topLeftCell.x + (size.x - 1), rect.topLeftCell.y + (size.y - 1)};
		return rect;
	}

	Utility::Vector2<int> ToCell(const Utility::Vector2<float>& position)
	{
		return { static_cast<int>(std::floor(position.x)), static_cast<int>(std::floor(position.y))};
	}

	// Algorithme de Bresenham : parcourt les cases entre le depart et l'arrivee
	// uniquement avec dees entiers
	bool CrossesRect(const Utility::Vector2<float>& from, const Utility::Vector2<float>& to, const CellRect& target)
	{
		const Utility::Vector2<int> end = ToCell(to);
		Utility::Vector2<int> cell = ToCell(from);

		const int distanceX = std::abs(end.x - cell.x);
		const int distanceY = std::abs(end.y - cell.y);
		const int stepX = (cell.x < end.x) ? 1 : -1;
		const int stepY = (cell.y < end.y) ? 1 : -1;


		int error = distanceX - distanceY;

		while (true)
		{
			if (target.Contains(cell))
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
