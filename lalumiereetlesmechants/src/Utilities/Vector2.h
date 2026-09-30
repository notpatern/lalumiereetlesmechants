#pragma once

namespace Utilities {
    template <typename T> struct Vector2 {
        T x;
        T y;

        Vector2() : x(0), y(0) {}
        Vector2(T x, T y) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2 operator-(const Vector2& other) const {
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

