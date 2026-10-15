#include "GameScreen.h"
#include "DebugConsole.h"

#include <string>

#include "../Gameplay/PlayArea.h"
#include "../Gameplay/Collision.h"

void GameScreen::Update(float deltaTime, const InputManager& input)
{
	m_request = ScreenRequest::None;

	if (m_gameManager.IsGameOver() || m_gameManager.IsVictory())
	{
		switch (m_endMenu.Update(input))
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

	if (input.WasJustPressed(VK_ESCAPE))
	{
		m_isPaused = !m_isPaused;
	}

	if (m_isPaused)
	{
		switch (m_pauseMenu.Update(input))
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

	m_gameManager.Update(deltaTime, input);
}

std::vector<ScreenText> GameScreen::getTexts() const
{
	std::vector<ScreenText> texts;

	texts.push_back({ "Score : " + std::to_string(m_gameManager.getScore()), { 0, HUD_ROW } });
	texts.push_back({ "Vies : " + std::to_string(m_gameManager.getPlayer().getHealth()), { LIVES_COLUMN, HUD_ROW } });

	if (m_gameManager.IsGameOver())
	{
		texts.push_back(CenteredText(GAME_OVER_TEXT, 10));
		m_endMenu.AddTexts(texts, 13);
	}
	else if (m_gameManager.IsVictory())
	{
		texts.push_back(CenteredText(VICTORY_TEXT, 9));
		texts.push_back(CenteredText("Score : " + std::to_string(m_gameManager.getScore()), 11));
		m_endMenu.AddTexts(texts, 14);
	}
	else if (m_isPaused)
	{
		texts.push_back(CenteredText(PAUSE_TEXT, 10));
		m_pauseMenu.AddTexts(texts, 13);
	}

	return texts;
}

void GameScreen::DrawDebug(HANDLE console) const
{
	DebugConsole::Clear(console);
	const Player& player = m_gameManager.getPlayer();

	DWORD written = 0;
	auto draw = [&](const Utility::Vector2<float>& position, char character)
	{
		const Utility::Vector2<int> cellPosition = Collision::ToCell(position);
		if (!PlayArea::Bounds().Contains(cellPosition))
		{
			return;
		}
		const COORD cell{static_cast<SHORT>(cellPosition.x), static_cast<SHORT>(cellPosition.y)};
		FillConsoleOutputCharacterA(console, character, 1, cell, &written);
		FillConsoleOutputAttribute(console, 0x0F, 1, cell, &written);
	};

	draw(player.getPosition(), '@');
	int activeMissiles = 0;
	m_gameManager.getMissiles().ForEachActive([&](const Missile& missile)
	{
		const Utility::Vector2<int> size = missile.getHitboxSize();
		const bool isBig = size.x > 1 || size.y > 1;
		const char character = isBig ? 'O' : (missile.getTeam() == Team::Enemy ? 'o' : '|');
		for (int y = 0; y < size.y; ++y)
		{
			for (int x = 0; x < size.x; ++x)
			{
				draw({ missile.getCurrentPosition().x + x, missile.getCurrentPosition().y + y }, character);
			}
		}
		++activeMissiles;
	});

	SetConsoleTitleA(("Missiles actifs : " + std::to_string(activeMissiles)).c_str());

	m_gameManager.getEnemies().ForEachActive([&](const Enemy& enemy)
	{
		const Collision::CellRect rect = enemy.getHitboxRect();
		for (int y = rect.topLeftCell.y; y <= rect.bottomRightCell.y; ++y)
		{
			for (int x = rect.topLeftCell.x; x <= rect.bottomRightCell.x; ++x)
			{
				draw({ static_cast<float>(x), static_cast<float>(y) }, '.');
			}
		}

		draw(enemy.getCurrentPosition(), enemy.IsFlashing() ? '*' : EnemyDebugCharacter(enemy.getType()));
	});

	DebugConsole::WriteTexts(console, getTexts());
}

char GameScreen::EnemyDebugCharacter(EnemyType type)
{
	switch (type)
	{
	case EnemyType::Turret:
		return 'T';
	case EnemyType::Hunter:
	default:
		return 'C';
	}
}
