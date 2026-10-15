#include "CreditsScreen.h"
#include "DebugConsole.h"

void CreditsScreen::Update(float, const InputManager& input)
{
	const bool wantsBack = m_menu.Update(input) != Menu::NO_CHOICE || input.WasJustPressed(VK_ESCAPE);
	m_request = wantsBack ? ScreenRequest::Title : ScreenRequest::None;
}

std::vector<ScreenText> CreditsScreen::getTexts() const
{
	std::vector<ScreenText> texts;
	for (int i = 0; i < static_cast<int>(LINES.size()); ++i)
	{
		texts.push_back(CenteredText(LINES[i], 5 + i));
	}
	m_menu.AddTexts(texts, 20);
	return texts;
}

void CreditsScreen::DrawDebug(HANDLE console) const
{
	DebugConsole::Clear(console);
	DebugConsole::WriteTexts(console, getTexts());

	SetConsoleTitleA("Credits");
}
