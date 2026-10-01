#pragma once

#include "../Utilities/Timer.h"
#include "../Utilities/Vector2.h"
#include "World/World.h"
#include "Render/Renderer.h"

class App {
private:
	Utility::Timer* m_timer;
	bool m_isRunning{true};

	CONSOLE_SCREEN_BUFFER_INFO m_consoleInfo;
	HANDLE m_consoleHandle;

	Utility::Vector2<int> m_windowSize{};

	World m_world{};
	Renderer m_renderer;

public:
	App(Utility::Timer* timer);
	~App();

	void Run();	
	LONG_PTR setConsoleWindowStyle(INT n_index, LONG_PTR new_style);

	inline bool getIsRunning() {
		return m_isRunning;
	}
};
