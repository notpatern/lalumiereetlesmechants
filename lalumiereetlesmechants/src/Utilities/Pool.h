#pragma once
#include <deque>
#include <concepts>
#include <cstddef>

namespace Utility
{
	template <typename T>
	concept Poolable = std::default_initializable<T> && requires(const T& inObject)
	{
		{ inObject.IsActive() } -> std::same_as<bool>;
	};

	template <typename T>
	concept Updatable = requires(T& outObject, float inDeltaTime)
	{
		outObject.Update(inDeltaTime);
	};

	template <Poolable T>
	class Pool
	{
	public:
		Pool(std::size_t inInitialSize, std::size_t inMaxSize);

		[[nodiscard]] T* Acquire();

		void Update(float inDeltaTime) requires Updatable<T>
		{
			ForEachActive([inDeltaTime](T& outObject) { outObject.Update(inDeltaTime); });
		}

		template <std::invocable<T&> Function>
		void ForEachActive(Function inFunction)
		{
			for (std::size_t i = 0; i < m_objects.size(); ++i)
			{
				if (m_objects[i].IsActive())
				{
					inFunction(m_objects[i]);
				}
			}
		}

		template <std::invocable<const T&> Function>
		void ForEachActive(Function inFunction) const
		{
			for (std::size_t i = 0; i < m_objects.size(); ++i)
			{
				if (m_objects[i].IsActive())
				{
					inFunction(m_objects[i]);
				}
			}
		}

	private:
		std::deque<T> m_objects;
		std::size_t m_maxSize;
		std::size_t m_growthStep;
		std::size_t m_searchStart;

		void Grow();
	};

	template <Poolable T>
	Pool<T>::Pool(std::size_t inInitialSize, std::size_t inMaxSize) : m_objects(inInitialSize), m_maxSize(inMaxSize), m_growthStep(inInitialSize > 0 ? inInitialSize : 1), m_searchStart(0)
	{
	}

	template <Poolable T>
	T* Pool<T>::Acquire()
	{
		const std::size_t count = m_objects.size();
		for (std::size_t checked = 0; checked < count; ++checked)
		{
			const std::size_t index = (m_searchStart + checked) % count;
			if (!m_objects[index].IsActive())
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
