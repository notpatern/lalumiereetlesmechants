#include "TitleScreen.h"
#include "../Gameplay/PlayArea.h"

void TitleScreen::Update(float, const InputManager& input)
{
	m_request = input.WasJustPressed(VK_SPACE) ? ScreenRequest::NewGame : ScreenRequest::None;
}

void TitleScreen::Draw(Renderer& renderer) const
{
	//TODO titre du jeu + space
}

void TitleScreen::DrawDebug(HANDLE console) const
{
	DWORD written = 0;
	FillConsoleOutputCharacterA(console, ' ', PlayArea::WIDTH * PlayArea::HEIGHT, { 0, 0 }, &written);

	const SHORT titleColumn = static_cast<SHORT>((PlayArea::WIDTH - static_cast<int>(TITLE_TEXT.size())) / 2);
	const SHORT startColumn = static_cast<SHORT>((PlayArea::WIDTH - static_cast<int>(START_TEXT.size())) / 2);
	WriteConsoleOutputCharacterA(console, TITLE_TEXT.data(), static_cast<DWORD>(TITLE_TEXT.size()), { titleColumn, 10 }, &written);
	WriteConsoleOutputCharacterA(console, START_TEXT.data(), static_cast<DWORD>(START_TEXT.size()), { startColumn, 14 }, &written);

	SetConsoleTitleA("La lumiere et les mechants");
}
