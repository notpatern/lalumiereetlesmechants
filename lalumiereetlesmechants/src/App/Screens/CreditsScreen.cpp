#include "CreditsScreen.h"

void CreditsScreen::Update(float, const InputManager& inInput)
{
	const bool wantsBack = m_menu.Update(inInput) != Menu::NO_CHOICE || inInput.WasJustPressed(VK_ESCAPE);
	m_request = wantsBack ? ScreenRequest::Title : ScreenRequest::None;
}

std::vector<ScreenText> CreditsScreen::getTexts() const
{
	std::vector<ScreenText> texts;
	for (int i = 0; i < static_cast<int>(LINES.size()); ++i)
	{
		texts.push_back(MakeCenteredText(LINES[i], 5 + i));
	}
	m_menu.AddTexts(texts, 20);
	return texts;
}
