#pragma once

#include "../Utilities/Timer.h"

class App {
private:
	Utilities::Timer* m_timer;
	bool m_isRunning{true};
public:
	App(Utilities::Timer* timer);
	~App();

	void Run();
	inline bool getIsRunning() {
		return m_isRunning;
	}
};
