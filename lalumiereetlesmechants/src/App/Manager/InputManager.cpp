#include "InputManager.h"
#include <WinUser.h>

bool InputManager::isKeyDown(int virtualKey)
{
	return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}
