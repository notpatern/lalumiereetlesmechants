#pragma once

namespace Utilities {
	template <typename T>
	class Vector2
	{
	public:
		Vector2(T x, T y);
		~Vector2();
		inline T getX() {
			return m_x;
		}
		inline T getT() {
			return m_y;
		}

		inline T setX(T x) {
			m_x = x;
		}
		inline T setY(T y) {
			m_y = y;
		}

	private:
		T m_x;
		T m_y;

	};
}

