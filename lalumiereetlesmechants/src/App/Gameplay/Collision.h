#pragma once
#include "../../Utilities/Vector2.h"

namespace Collision
{
	struct CellRect
	{
		Utility::Vector2<int> topLeftCell;
		Utility::Vector2<int> bottomRightCell;

		bool ContainsCell(const Utility::Vector2<int>& inCell) const
		{
			return inCell.x >= topLeftCell.x && inCell.x <= bottomRightCell.x && inCell.y >= topLeftCell.y && inCell.y <= bottomRightCell.y;
		}

		bool ContainsRect(const CellRect& inOther) const
		{
			return ContainsCell(inOther.topLeftCell) && ContainsCell(inOther.bottomRightCell);
		}

		bool Overlaps(const CellRect& inOther) const
		{
			return topLeftCell.x <= inOther.bottomRightCell.x && inOther.topLeftCell.x <= bottomRightCell.x
			&& topLeftCell.y <= inOther.bottomRightCell.y && inOther.topLeftCell.y <= bottomRightCell.y;
		}

		Utility::Vector2<float> getCenter() const
		{
			return { (topLeftCell.x + bottomRightCell.x + 1) / 2.f, (topLeftCell.y + bottomRightCell.y + 1) / 2.f };
		}
	};

	struct Hitbox
	{
		Utility::Vector2<int> offset{ 0, 0 }; // decalage depuis la position (qui est coin haut gauche du sprite)
		Utility::Vector2<int> size{ 1, 1 };

		CellRect At(const Utility::Vector2<float>& inPosition) const;
	};

	Utility::Vector2<int> ToCell(const Utility::Vector2<float>& inPosition);

	bool CrossesRect(const Utility::Vector2<float>& inFrom, const Utility::Vector2<float>& inTo, const CellRect& inTarget);
}
