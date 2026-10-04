#pragma once

#include "../../Utilities/Vector2.h"

namespace Collision
{
	struct CellRect
	{
		Utility::Vector2<int> topLeftCell;
		Utility::Vector2<int> bottomRightCell;

		bool Contains(const Utility::Vector2<int>& cell) const
		{
			return cell.x >= topLeftCell.x && cell.x <= bottomRightCell.x && cell.y >= topLeftCell.y && cell.y <= bottomRightCell.y;
		}

		bool Overlaps(const CellRect& other) const
		{
			return topLeftCell.x <= other.bottomRightCell.x && other.topLeftCell.x <= bottomRightCell.x
			&& topLeftCell.y <= other.bottomRightCell.y && other.topLeftCell.y <= bottomRightCell.y;
		}
	};

	struct Hitbox
	{
		Utility::Vector2<int> offset{0,0};        // decalage depuis la position (qui est coin haut gauche du sprite)
		Utility::Vector2<int> size{ 1, 1 };

		CellRect At(const Utility::Vector2<float>& position) const;
	};


	Utility::Vector2<int> ToCell(const Utility::Vector2<float>& position);

	bool CrossesRect(const Utility::Vector2<float>& from, const Utility::Vector2<float>& to, const CellRect& target);

}

