#include "DebugConsole.h"
#include "../Gameplay/PlayArea.h"

namespace DebugConsole
{
	void Clear(HANDLE console)
	{
		CONSOLE_SCREEN_BUFFER_INFO info{};
		GetConsoleScreenBufferInfo(console, &info);

		DWORD written = 0;
		FillConsoleOutputCharacterA(console, ' ', info.dwSize.X * (PlayArea::HEIGHT + 2), { 0, 0 }, &written);
	}

	void WriteTexts(HANDLE console, const std::vector<ScreenText>& texts)
	{
		DWORD written = 0;
		for (const ScreenText& text : texts)
		{
			const COORD cell{ static_cast<SHORT>(text.position.x), static_cast<SHORT>(text.position.y) };
			WriteConsoleOutputCharacterA(console, text.text.data(), static_cast<DWORD>(text.text.size()), cell, &written);
		}
	}
}
