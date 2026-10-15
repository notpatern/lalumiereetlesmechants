#include "TitleScreen.h"
#include "DebugConsole.h"

void TitleScreen::Update(float, const InputManager& input)
{
	switch (m_menu.Update(input))
	{
	case PLAY_CHOICE:
		m_request = ScreenRequest::NewGame;
		break;
	case CREDITS_CHOICE:
		m_request = ScreenRequest::Credits;
		break;
	case QUIT_CHOICE:
		m_request = ScreenRequest::Quit;
		break;
	default:
		m_request = ScreenRequest::None;
		break;
	}
}

void TitleScreen::Draw(Renderer& renderer) const
{
	//TODO titre du jeu + space
}

void TitleScreen::DrawDebug(HANDLE console) const
{
	DebugConsole::Clear(console);
	DebugConsole::WriteCentered(console, TITLE_TEXT, 10);
	m_menu.DrawDebug(console, 14);

	SetConsoleTitleA("La lumiere et les mechants");
}
