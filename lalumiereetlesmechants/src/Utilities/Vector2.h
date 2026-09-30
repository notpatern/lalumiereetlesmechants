#pragma once

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

	Vector2<T>* GetPosition();

	void Clear();

private:
	T m_x;
	T m_y;

};

