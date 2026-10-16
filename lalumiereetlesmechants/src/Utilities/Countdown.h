#pragma once

namespace Utility
{
	class Countdown
	{
	public:
		void Start(float inDuration)
		{
			m_remaining = inDuration;
		}

		void Chain(float inDuration)
		{
			m_remaining += inDuration;
		}

		void StartIfLonger(float inDuration)
		{
			if (inDuration > m_remaining)
			{
				m_remaining = inDuration;
			}
		}

		void Update(float inDeltaTime)
		{
			m_remaining -= inDeltaTime;
		}

		bool IsFinished() const { return m_remaining <= 0.f; }
		float getRemaining() const { return m_remaining > 0.f ? m_remaining : 0.f; }

	private:
		float m_remaining = 0.f;
	};
}
