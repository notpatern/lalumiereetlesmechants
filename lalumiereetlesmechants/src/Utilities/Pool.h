#pragma once
#include <concepts>
#include <cstddef>
#include <memory>
#include <vector>

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

	// pas de new pendant la partie : au plafond Acquire rend nullptr, on prefere rater un tir qu'allouer en plein jeu
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
			for (std::size_t chunkIndex = 0; chunkIndex < m_chunks.size(); ++chunkIndex)
			{
				const std::size_t chunkSize = GetChunkSize(chunkIndex);
				for (std::size_t i = 0; i < chunkSize; ++i)
				{
					T& object = m_chunks[chunkIndex][i];
					if (object.IsActive())
					{
						inFunction(object);
					}
				}
			}
		}

		template <std::invocable<const T&> Function>
		void ForEachActive(Function inFunction) const
		{
			for (std::size_t chunkIndex = 0; chunkIndex < m_chunks.size(); ++chunkIndex)
			{
				const std::size_t chunkSize = GetChunkSize(chunkIndex);
				for (std::size_t i = 0; i < chunkSize; ++i)
				{
					const T& object = m_chunks[chunkIndex][i];
					if (object.IsActive())
					{
						inFunction(object);
					}
				}
			}
		}

	private:
		// des paquets et pas un vector : le renderer garde l'adresse des objets, ils ne doivent jamais bouger
		std::vector<std::unique_ptr<T[]>> m_chunks;
		std::size_t m_size;
		std::size_t m_maxSize;
		std::size_t m_growthStep;
		std::size_t m_searchStart;

		void Grow();
		T& GetObjectAt(std::size_t inIndex);
		std::size_t GetChunkSize(std::size_t inChunkIndex) const;
	};

	template <Poolable T>
	Pool<T>::Pool(std::size_t inInitialSize, std::size_t inMaxSize) : m_size(0), m_maxSize(inMaxSize), m_growthStep(inInitialSize > 0 ? inInitialSize : 1), m_searchStart(0)
	{
		if (inInitialSize > 0)
		{
			Grow();
		}
	}

	template <Poolable T>
	T* Pool<T>::Acquire()
	{
		for (std::size_t checked = 0; checked < m_size; ++checked)
		{
			const std::size_t index = (m_searchStart + checked) % m_size;
			T& object = GetObjectAt(index);
			if (!object.IsActive())
			{
				m_searchStart = index + 1;
				return &object;
			}
		}

		if (m_size >= m_maxSize)
		{
			return nullptr;
		}

		const std::size_t firstNewIndex = m_size;

		Grow();
		m_searchStart = firstNewIndex + 1;

		return &GetObjectAt(firstNewIndex);
	}

	template <Poolable T>
	void Pool<T>::Grow()
	{
		std::size_t chunkSize = m_growthStep;
		if (m_size + chunkSize > m_maxSize)
		{
			chunkSize = m_maxSize - m_size;
		}
		m_chunks.push_back(std::make_unique<T[]>(chunkSize));
		m_size += chunkSize;
	}

	template <Poolable T>
	T& Pool<T>::GetObjectAt(std::size_t inIndex)
	{
		return m_chunks[inIndex / m_growthStep][inIndex % m_growthStep];
	}

	template <Poolable T>
	std::size_t Pool<T>::GetChunkSize(std::size_t inChunkIndex) const
	{
		const std::size_t remaining = m_size - inChunkIndex * m_growthStep;
		return remaining < m_growthStep ? remaining : m_growthStep;
	}
}
