#include "CreditsScreen.h"
#include "DebugConsole.h"

void CreditsScreen::Update(float, const InputManager& input)
{
	const bool wantsBack = m_menu.Update(input) != Menu::NO_CHOICE || input.WasJustPressed(VK_ESCAPE);
	m_request = wantsBack ? ScreenRequest::Title : ScreenRequest::None;
}

void CreditsScreen::Draw(Renderer& renderer) const
{
	//TODO ecrire options
}

void CreditsScreen::DrawDebug(HANDLE console) const
{
	DebugConsole::Clear(console);

	for (int i = 0; i < static_cast<int>(LINES.size()); ++i)
	{
		DebugConsole::WriteCentered(console, LINES[i], 5 + i);
	}

	m_menu.DrawDebug(console, 20);

	SetConsoleTitleA("Credits");
}
