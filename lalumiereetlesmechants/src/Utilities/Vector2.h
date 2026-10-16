#pragma once

namespace Utility
{
	template <typename T>
	struct Vector2
	{
		T x;
		T y;

		Vector2() : x(0), y(0)
		{
		}

		Vector2(T inX, T inY) : x(inX), y(inY)
		{
		}

		Vector2(const Vector2& inOther) : x(inOther.x), y(inOther.y)
		{
		}

		static const Vector2 zero()
		{
			return Vector2(0, 0);
		}

		/// <summary>
		/// Be careful using this with floating point types.
		/// </summary>
		/// <param name="inOther"></param>
		bool operator==(const Vector2& inOther) const
		{
			return x == inOther.x && y == inOther.y;
		}

		/// <summary>
		/// Be careful using this with floating point types.
		/// </summary>
		/// <param name="inOther"></param>
		bool operator!=(const Vector2& inOther) const
		{
			return x != inOther.x || y != inOther.y;
		}

		Vector2& operator=(const Vector2& inOther)
		{
			x = inOther.x;
			y = inOther.y;
			return *this;
		}

		Vector2 operator+(const Vector2& inOther) const
		{
			return Vector2(x + inOther.x, y + inOther.y);
		}

		Vector2 operator-(const Vector2& inOther) const
		{
			return Vector2(x - inOther.x, y - inOther.y);
		}

		Vector2& operator+=(const Vector2& inOther)
		{
			x += inOther.x;
			y += inOther.y;
			return *this;
		}

		Vector2& operator-=(const Vector2& inOther)
		{
			x -= inOther.x;
			y -= inOther.y;
			return *this;
		}
	};
}
