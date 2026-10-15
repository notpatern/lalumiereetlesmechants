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

	void WriteCentered(HANDLE console, std::string_view text, int row)
	{
		DWORD written = 0;
		const SHORT column = static_cast<SHORT>((PlayArea::WIDTH - static_cast<int>(text.size())) / 2);
		WriteConsoleOutputCharacterA(console, text.data(), static_cast<DWORD>(text.size()), { column, static_cast<SHORT>(row) }, &written);
	}
}
