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

	//TODO : Faire un event et mettre en prive

	void Update();

	Utility::Vector2<int>& getDesiredDirection();

private:
	int m_key_count = 256;
	bool m_current[256]{};
	bool m_previous[256]{};

	Utility::Vector2<int> m_inputDesiredDirection{};

	const Utility::Vector2<int>& ReadMove();
};

