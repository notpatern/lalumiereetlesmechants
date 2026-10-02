#pragma once
#include <deque>
#include <concepts>
#include <cstddef>

namespace Utility
{
	template <typename T>
	concept Poolable = std::default_initializable<T> && requires(T& object, float dt)
	{
		{ object.getIsActive() } -> std::same_as<bool>;
		object.Update(dt);
	};

	template <Poolable T>
	class Pool
	{
	public:
		Pool(std::size_t initialSize, std::size_t maxSize);
		[[nodiscard]] T* Acquire();

		void Update(float dt);

		std::deque<T>& getObjects() { return m_objects; }
		const std::deque<T>& getObjects() const { return m_objects; }

	private:
		std::deque<T> m_objects;
		std::size_t m_maxSize;
		std::size_t m_growthStep;
		std::size_t m_searchStart;

		void Grow();

	};

	template <Poolable T>
	Pool<T>::Pool(std::size_t initialSize, std::size_t maxSize) : m_objects(initialSize), m_maxSize(maxSize), m_growthStep(initialSize > 0 ? initialSize : 1), m_searchStart(0)
	{

	}

	template <Poolable T>
	T* Pool<T>::Acquire()
	{
		const std::size_t count = m_objects.size();
		for (std::size_t checked = 0; checked < count; ++checked)
		{
			const std::size_t index = (m_searchStart + checked) % count;
			if (!m_objects[index].getIsActive())
			{
				m_searchStart = index + 1;
				return &m_objects[index];
			}
		}

		if (m_objects.size() >= m_maxSize)
		{
			return nullptr;
		}

		const std::size_t firstNewIndex = m_objects.size();

		Grow();
		m_searchStart = firstNewIndex + 1;

		return &m_objects[firstNewIndex];
	}

	template <Poolable T>
	void Pool<T>::Update(float dt)
	{
		for (std::size_t i = 0; i < m_objects.size(); ++i)
		{
			if (m_objects[i].getIsActive())
			{
				m_objects[i].Update(dt);
			}
		}
	}

	template <Poolable T>
	void Pool<T>::Grow()
	{
		std::size_t newSize = m_objects.size() + m_growthStep;
		if (newSize > m_maxSize)
		{
			newSize = m_maxSize;
		}
		m_objects.resize(newSize);
	}
}
