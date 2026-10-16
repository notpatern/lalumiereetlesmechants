#pragma once
#include "Countdown.h"
#include "Vector2.h"

namespace Utility
{
	class ScreenShake
	{
	public:
		void Start(const Vector2<int>& amplitude, float duration)
		{
			if (m_timer.IsFinished())
			{
				m_amplitude = { 0, 0 };
			}

			if (amplitude.x > m_amplitude.x)
			{
				m_amplitude.x = amplitude.x;
			}
			if (amplitude.y > m_amplitude.y)
			{
				m_amplitude.y = amplitude.y;
			}
			m_timer.StartIfLonger(duration);
		}

		void Update(float realDeltaTime) { m_timer.Update(realDeltaTime); }

		Vector2<int> getOffset() const
		{
			if (m_timer.IsFinished())
			{
				return { 0, 0 };
			}

			return { Alternate(m_amplitude.x, X_FLIP_PERIOD), Alternate(m_amplitude.y, Y_FLIP_PERIOD) };
		}

	private:
		static constexpr float X_FLIP_PERIOD = 0.03f;
		static constexpr float Y_FLIP_PERIOD = 0.06f;

		Vector2<int> m_amplitude;
		Countdown m_timer;

		int Alternate(int amplitude, float flipPeriod) const
		{
			const int flipCount = static_cast<int>(m_timer.getRemaining() / flipPeriod);
			return flipCount % 2 == 0 ? amplitude : -amplitude;
		}
	};
}
