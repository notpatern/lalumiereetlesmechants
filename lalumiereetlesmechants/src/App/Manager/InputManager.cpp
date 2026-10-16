#include "InputManager.h"
#include <windows.h>

void InputManager::Update()
{
	m_previousKeys = m_currentKeys;

	if (HasConsoleFocus())
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

bool InputManager::IsDown(int inVirtualKey) const
{
	return inVirtualKey >= 0 && inVirtualKey < KEY_COUNT && m_currentKeys[inVirtualKey];
}

bool InputManager::WasJustPressed(int inVirtualKey) const
{
	return IsDown(inVirtualKey) && !m_previousKeys[inVirtualKey];
}

// GetAsyncKeyState renvoie 16 bits
// le bit 0x8000 vaut 1 si la touche est enfoncee en ce moment
bool InputManager::ReadKeyFromWindows(int inVirtualKey)
{
	return (GetAsyncKeyState(inVirtualKey) & 0x8000) != 0;
}

bool InputManager::HasConsoleFocus()
{
	// always true parce que la CMD et le TERMINAL windows ne se comportent pas pareil et la scope du projet ne requiert pas d'archi plus complexe.
	return true;
}
