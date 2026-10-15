#pragma once
#include "ScreenText.h"
#include <vector>
#include <windows.h>

namespace DebugConsole
{
	void Clear(HANDLE console);
	void WriteTexts(HANDLE console, const std::vector<ScreenText>& texts);
}
