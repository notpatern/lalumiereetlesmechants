#pragma once
#include <array>
class InputManager
{
public:
	bool IsKeyDown(int virtualKey);
	void test();

	bool ConsoleHasFocus();
	bool IsDown(int vitrtualKey);
	bool WasJustPressed(int virtualKey);

	void Update();

private:
	int keyCount = 256;
	std::array<bool, 256> m_current{};
	std::array<bool, 256> m_previous{};
};

