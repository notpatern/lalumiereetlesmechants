#include "InputManager.h"
#include <windows.h>

bool InputManager::IsKeyDown(int virtualKey)
{
	return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
	return true;
}

bool InputManager::ConsoleHasFocus()
{
	return GetForegroundWindow() == GetAncestor(GetConsoleWindow(), GA_ROOTOWNER);
	return true;
}

bool InputManager::IsDown(int virtualKey)
{
    return virtualKey >= 0 && virtualKey < keyCount && m_current[virtualKey];
}

bool InputManager::WasJustPressed(int virtualKey)
{
    return IsDown(virtualKey) && !m_previous[virtualKey];
}

void InputManager::Update()
{
	m_previous = m_current;
	bool focus = ConsoleHasFocus();

	if (focus) {
		for (int key = 0; key < keyCount; ++key) {
			m_current[key] = IsKeyDown(key);
		}
	}
}