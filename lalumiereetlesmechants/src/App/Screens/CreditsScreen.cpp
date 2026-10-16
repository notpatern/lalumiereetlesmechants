#include "CreditsScreen.h"

void CreditsScreen::Update(float, const InputManager& inInput)
{
	const bool wantsBack = m_menu.Update(inInput) != Menu::NO_CHOICE || inInput.WasJustPressed(VK_ESCAPE);
	m_request = wantsBack ? ScreenRequest::Title : ScreenRequest::None;
}

void CreditsScreen::AddTexts(std::vector<ScreenText>& outTexts) const
{
	for (int i = 0; i < static_cast<int>(LINES.size()); ++i)
	{
		outTexts.push_back(MakeCenteredText(LINES[i], 5 + i));
	}
	m_menu.AddTexts(outTexts, 20);
}
