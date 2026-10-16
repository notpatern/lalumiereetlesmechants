#include "GameScreen.h"
#include <string>

void GameScreen::Update(float inDeltaTime, const InputManager& inInput)
{
	m_request = ScreenRequest::None;

	if (m_gameManager.IsGameOver() || m_gameManager.IsVictory())
	{
		m_gameManager.Update(inDeltaTime, inInput);
		m_endElapsed += inDeltaTime;
		if (m_endElapsed < END_INPUT_DELAY)
		{
			return;
		}

		switch (m_endMenu.Update(inInput))
		{
		case END_RESTART_CHOICE:
			m_request = ScreenRequest::NewGame;
			break;
		case END_MENU_CHOICE:
			m_request = ScreenRequest::Title;
			break;
		}
		return;
	}

	if (inInput.WasJustPressed(VK_ESCAPE))
	{
		m_isPaused = !m_isPaused;
	}

	if (m_isPaused)
	{
		switch (m_pauseMenu.Update(inInput))
		{
		case PAUSE_RESUME_CHOICE:
			m_isPaused = false;
			break;
		case PAUSE_RESTART_CHOICE:
			m_request = ScreenRequest::NewGame;
			break;
		case PAUSE_MENU_CHOICE:
			m_request = ScreenRequest::Title;
			break;
		}
		return;
	}

#ifdef _DEBUG
	if (inInput.WasJustPressed(VK_F2))
	{
		m_gameManager.SkipWave();
	}
#endif

	m_gameManager.Update(inDeltaTime, inInput);
	m_hud.Update(inDeltaTime);
}

std::vector<ScreenText> GameScreen::getTexts() const
{
	std::vector<ScreenText> texts;

	m_hud.AddTexts(texts, m_gameManager);

	if (m_gameManager.IsGameOver())
	{
		texts.push_back(MakeCenteredText(GAME_OVER_TEXT, 10));
		m_endMenu.AddTexts(texts, 13);
	}
	else if (m_gameManager.IsVictory())
	{
		texts.push_back(MakeCenteredText(VICTORY_TEXT, 9));
		texts.push_back(MakeCenteredText("Score : " + std::to_string(m_gameManager.getScore()), 11));
		m_endMenu.AddTexts(texts, 14);
	}
	else if (m_isPaused)
	{
		texts.push_back(MakeCenteredText(PAUSE_TEXT, 10));
		m_pauseMenu.AddTexts(texts, 13);
	}

	return texts;
}
