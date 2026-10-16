#pragma once
#include "Countdown.h"
#include "Vector2.h"

namespace Utility
{
	class ScreenShake
	{
	public:
		void Start(const Vector2<int>& inAmplitude, float inDuration)
		{
			if (m_timer.IsFinished())
			{
				m_amplitude = { 0, 0 };
			}

			if (inAmplitude.x > m_amplitude.x)
			{
				m_amplitude.x = inAmplitude.x;
			}
			if (inAmplitude.y > m_amplitude.y)
			{
				m_amplitude.y = inAmplitude.y;
			}
			m_timer.StartIfLonger(inDuration);
		}

		void Update(float inRealDeltaTime)
		{
			m_timer.Update(inRealDeltaTime);
		}

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

		int Alternate(int inAmplitude, float inFlipPeriod) const
		{
			const int flipCount = static_cast<int>(m_timer.getRemaining() / inFlipPeriod);
			return flipCount % 2 == 0 ? inAmplitude : -inAmplitude;
		}
	};
}
