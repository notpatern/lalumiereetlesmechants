#include "InputManager.h"

#include <iostream>
#include <windows.h>

bool InputManager::IsKeyDown(int virtualKey)
{
	return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

bool InputManager::ConsoleHasFocus()
{
	return GetForegroundWindow() == GetAncestor(GetConsoleWindow(), GA_ROOTOWNER);
}

bool InputManager::IsDown(int virtualKey)
{
	return virtualKey >= 0 && virtualKey < m_key_count && m_current[virtualKey];
}

bool InputManager::WasJustPressed(int virtualKey)
{
	return IsDown(virtualKey) && !m_previous[virtualKey];
}

void InputManager::Update()
{
	m_previous = m_current;
	bool focus = ConsoleHasFocus();

	if (focus)
	{
		for (int key = 0; key < m_key_count; ++key)
		{
			m_current[key] = IsKeyDown(key);
		}
	}

	m_desiredDirection = ReadMove();
	std::cout<<m_desiredDirection.x<<" "<<m_desiredDirection.y<< std::endl;
}

Utility::Vector2<int> InputManager::getDesiredDirection()
{
	// Je voulais faire en inline mais au final non AU CAS OU
	return m_desiredDirection;
}

Utility::Vector2<int> InputManager::ReadMove()
{
	if (IsDown(VK_UP) || IsDown('Z'))
	{
		return { 0, -1 };
	}
	if (IsDown(VK_DOWN) || IsDown('S'))
	{
		return { 0, 1 };
	}
	if (IsDown(VK_LEFT) || IsDown('Q') || IsDown('A'))
	{
		return { -1, 0 };
	}
	if (IsDown(VK_RIGHT) || IsDown('D'))
	{
		return { 1, 0 };
	}
	return {0,0};
}
