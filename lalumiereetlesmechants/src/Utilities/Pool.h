#pragma once
#include <deque>
#include <concepts>

template<typename Tt> //TODO : Je l'utilise pas encore j'ai pas capte
concept HasGetIsActive = requires(const Tt& obj)
{
	{ obj.getIsActive() } -> std::same_as<bool>;
};

template <typename T>
class Pool
{
public:
	Pool(int initialSize, int maxSize);
	T* Acquire();

	void Update(float dt);

	std::deque<T>& GetObjects();
	//const std::deque<T>& GetObjects() const;

private:
	std::deque<T> m_objects;
	int m_maxSize;
	int m_growthStep;

	void Grow();

};

template <typename T>
Pool<T>::Pool(int initialSize, int maxSize) : m_objects(initialSize), m_maxSize(maxSize), m_growthStep(initialSize > 0 ? initialSize : 1)
{

}

template <typename T>
T* Pool<T>::Acquire()
{
	for (T& object : m_objects)
	{
		if (!object.getIsActive())
		{
			return &object;
		}
	}

	if (static_cast<int>(m_objects.size()) >= m_maxSize)
	{
		return nullptr;
	}

	const int firstNewIndex = static_cast<int>(m_objects.size());

	Grow();

	return &m_objects[firstNewIndex];
}

template <typename T>
void Pool<T>::Update(float dt)
{
	for (T& object : m_objects)
	{
		if (object.getIsActive())
		{
			object.Update(dt);
		}
	}
}

template <typename T>
std::deque<T>& Pool<T>::GetObjects()
{
	return m_objects;
}

template <typename T>
void Pool<T>::Grow()
{
	int newSize = static_cast<int>(m_objects.size()) + m_growthStep;
	if (newSize > m_maxSize)
	{
		newSize = m_maxSize;
	}
	m_objects.resize(newSize);
}
