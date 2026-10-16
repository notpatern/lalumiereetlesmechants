#include "TitleScreen.h"

void TitleScreen::Update(float, const InputManager& inInput)
{
	switch (m_menu.Update(inInput))
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

std::vector<ScreenText> TitleScreen::getTexts() const
{
	std::vector<ScreenText> texts;
	texts.push_back(MakeCenteredText(TITLE_TEXT, 10));
	m_menu.AddTexts(texts, 14);
	return texts;
}
