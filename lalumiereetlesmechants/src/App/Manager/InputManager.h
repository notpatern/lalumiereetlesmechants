#pragma once
#include <array>
#include "../../Utilities/Vector2.h"

class InputManager
{
public:
	bool IsKeyDown(int virtualKey);;

	bool ConsoleHasFocus();
	bool IsDown(int virtualKey);
	bool WasJustPressed(int virtualKey);

	void Update();

	inline Utility::Vector2<int> getDesiredDirection();

private:
	int m_key_count = 256;
	std::array<bool, 256> m_current{};
	std::array<bool, 256> m_previous{};

	Utility::Vector2<int> m_desiredDirection{};
	Utility::Vector2<int> m_currentDirection{};

	Utility::Vector2<int> ReadMove();
};

