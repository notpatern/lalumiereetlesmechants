#pragma once

#include <windows.h>

namespace Utilities {
	
	class Timer
	{
	public:
		LARGE_INTEGER lastUpdateTime;
		LONGLONG freq;

		Timer()
		{
			QueryPerformanceCounter(&lastUpdateTime);
			LARGE_INTEGER li_freq;
			QueryPerformanceFrequency(&li_freq);
			freq = li_freq.QuadPart;
			freq /= 1000;
		}

		void start(void)
		{
			QueryPerformanceCounter(&lastUpdateTime);
		}

		float getElapsedSeconds(bool restart = false)
		{
			LARGE_INTEGER timeNow;
			QueryPerformanceCounter(&timeNow);
			LONGLONG elapsedLong = timeNow.QuadPart - lastUpdateTime.QuadPart;

			float elapsed = (float)((float)elapsedLong / (float)freq);
			elapsed /= 1000.0f;

			if (restart)
				lastUpdateTime = timeNow;

			return elapsed;
		}

		unsigned long getElapsedMs(bool restart = false)
		{
			LARGE_INTEGER timeNow;
			QueryPerformanceCounter(&timeNow);
			LONGLONG elapsedLong = timeNow.QuadPart - lastUpdateTime.QuadPart;

			unsigned long elapsed = (unsigned long)((float)elapsedLong / (float)freq);
			return elapsed;
		}
	};
}