#pragma once

namespace Utility
{
	class Countdown
	{
	public:
		void Start(float duration) { m_remaining = duration; }
		void Update(float dt) { m_remaining -= dt; }
		bool IsFinished() const { return m_remaining <= 0.f; }

	private:
		float m_remaining = 0.f;
	};
}
