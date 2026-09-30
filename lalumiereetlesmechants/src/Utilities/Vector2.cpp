#include "Vector2.h"

template<typename T>
Utilities::Vector2<T>::Vector2(T x, T y) {
	m_x = x;
	m_y = y;
}

template<typename T>
Utilities::Vector2<T>::~Vector2()
{
}

template<typename T>
Utilities::Vector2<T>* Utilities::Vector2<T>::GetPosition()
{
	return this;
}

template<typename T>
void Utilities::Vector2<T>::Clear()
{
	m_x = 0;
	m_y = 0;
}

