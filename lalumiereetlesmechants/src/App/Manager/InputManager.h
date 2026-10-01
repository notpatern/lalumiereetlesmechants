#pragma once
#include <array>

class InputManager
{
public:
	void Update();
	bool IsDown(int virtualKey) const;
	bool WasJustPressed(int virtualKey) const;

private:
	static constexpr int KEY_COUNT = 256;

	std::array<bool, KEY_COUNT> m_currentKeys{};
	std::array<bool, KEY_COUNT> m_previousKeys{};

	static bool ReadKeyFromWindows(int virtualKey);
	static bool ConsoleHasFocus();
};
