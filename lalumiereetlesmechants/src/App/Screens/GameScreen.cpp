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

void GameScreen::Draw(Renderer& renderer) const
{
	//la jvais demander au renderer de dessiner les ennemis, les missiles et le joueur (sivisible), avec leurs sprites

	if (m_gameManager.IsGameOver())
	{
		DrawGameOver(renderer);
		m_endMenu.Draw(renderer, 13);
	}
	else if (m_gameManager.IsVictory())
	{
		DrawVictory(renderer);
		m_endMenu.Draw(renderer, 14);
	}
	else if (m_isPaused)
	{
		DrawPause(renderer);
		m_pauseMenu.Draw(renderer, 13);
	}
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
		draw(missile.getCurrentPosition(), '|');
		++activeMissiles;
	});

	SetConsoleTitleA(("Missiles actifs : " + std::to_string(activeMissiles)).c_str());

	m_gameManager.getEnemies().ForEachActive([&](const Enemy& enemy)
	{
		// Debug : un '.' sur chaque case de la hitbox, puis le W par-dessus
		const Collision::CellRect rect = enemy.getHitboxRect();
		for (int y = rect.topLeftCell.y; y <= rect.bottomRightCell.y; ++y)
		{
			for (int x = rect.topLeftCell.x; x <= rect.bottomRightCell.x; ++x)
			{
				draw({ static_cast<float>(x), static_cast<float>(y) }, '.');
			}
		}

		draw(enemy.getCurrentPosition(), enemy.IsFlashing() ? '*' : 'W');
	});


	if (m_gameManager.IsGameOver())
	{
		DebugConsole::WriteCentered(console, GAME_OVER_TEXT, 10);
		m_endMenu.DrawDebug(console, 13);
	}
	else if (m_gameManager.IsVictory())
	{
		DebugConsole::WriteCentered(console, VICTORY_TEXT, 9);
		DebugConsole::WriteCentered(console, "Score : " + std::to_string(m_gameManager.getScore()), 11);
		m_endMenu.DrawDebug(console, 14);
	}
	else if (m_isPaused)
	{
		DebugConsole::WriteCentered(console, PAUSE_TEXT, 10);
		m_pauseMenu.DrawDebug(console, 13);
	}
}

void GameScreen::DrawGameOver(Renderer& renderer)
{
	//la jvais ecrire le game over au milieu de la zone de jeu, et la touche R pour recommencer en dessous
}

void GameScreen::DrawVictory(Renderer& renderer) const
{
	//TODO : Render la victoire
}

void GameScreen::DrawPause(Renderer& renderer)
{
	//TODO : La pause
}

int GameScreen::CenteredColumn(std::string_view text)
{
	return (PlayArea::WIDTH - static_cast<int>(text.size())) / 2;
}
