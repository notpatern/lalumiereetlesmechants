#pragma once
#include <string_view>
#include <windows.h>

namespace DebugConsole
{
	void Clear(HANDLE console);
	void WriteCentered(HANDLE console, std::string_view text, int row);
}
