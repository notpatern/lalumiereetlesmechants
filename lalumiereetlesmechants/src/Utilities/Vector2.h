#pragma once

namespace Utility {
    template <typename T> struct Vector2 {
        T x;
        T y;

        Vector2() : x(0), y(0) {}
        Vector2(T x, T y) : x(x), y(y) {}

		Vector2(const Vector2& other) : x(other.x), y(other.y) {}

        static const Vector2 zero() {
            return Vector2(0, 0);
        };

		/// <summary>
		/// Be careful using this with floating point types.
		/// </summary>
		/// <param name="other"></param>
		bool operator==(const Vector2& other) const {
			return x == other.x && y == other.y;
		}

		/// <summary>
		/// Be careful using this with floating point types.
		/// </summary>
		/// <param name="other"></param>
		bool operator!=(const Vector2 & other) const{
			return x != other.x || y != other.y;
		}

		Vector2& operator=(const Vector2& other) {
			x = other.x;
			y = other.y;
			return *this;
		}

        Vector2& operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2& operator-(const Vector2& other) const {
            return Vector2(x - other.x, y - other.y);
        }

        Vector2& operator+=(const Vector2& other) const {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2& operator-=(const Vector2& other) const {
            x -= other.x;
            y -= other.y;
            return *this;
        }

    };
}

