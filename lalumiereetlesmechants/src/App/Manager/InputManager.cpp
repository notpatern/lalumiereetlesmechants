#include "InputManager.h"
#include <windows.h>

// GetAsyncKeyState renvoie 16 bits
// le bit 0x8000 vaut 1 si la touche est enfoncee en ce moment
bool InputManager::ReadKeyFromWindows(int virtualKey)
{
	return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

bool InputManager::ConsoleHasFocus()
{
	return GetForegroundWindow() == GetAncestor(GetConsoleWindow(), GA_ROOTOWNER);
}

bool InputManager::IsDown(int virtualKey) const
{
	return virtualKey >= 0 && virtualKey < KEY_COUNT && m_currentKeys[virtualKey];
}

bool InputManager::WasJustPressed(int virtualKey) const
{
	return IsDown(virtualKey) && !m_previousKeys[virtualKey];
}

void InputManager::Update()
{
	m_previousKeys = m_currentKeys;

	if (ConsoleHasFocus())
	{
		//Ici je lis 256 touches, donc un peu couteux
		//J'aurais pu enregistrer que les touches de notre jeu
		//Mais je prefere la reutilisabilite de la classe a la perf ici
		for (int key = 0; key < KEY_COUNT; ++key)
		{
			m_currentKeys[key] = ReadKeyFromWindows(key);
		}
	}
	else
	{
		m_currentKeys.fill(false);
	}
}
