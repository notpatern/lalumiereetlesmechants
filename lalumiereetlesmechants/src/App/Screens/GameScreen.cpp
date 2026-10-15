#include "GameScreen.h"
#include "DebugConsole.h"

#include <string>

#include "../Gameplay/PlayArea.h"

void GameScreen::Update(float deltaTime, const InputManager& input)
{
	m_request = ScreenRequest::None;

	if (m_gameManager.IsGameOver() || m_gameManager.IsVictory())
	{
		if (input.WasJustPressed('R'))
		{
			m_request = ScreenRequest::NewGame;
		}
		else if (input.WasJustPressed(VK_ESCAPE))
		{
			m_request = ScreenRequest::Title;
		}
		return;
	}

	if (input.WasJustPressed(VK_ESCAPE))
	{
		m_isPaused = !m_isPaused;
	}

	if (m_isPaused)
	{
		if (input.WasJustPressed('Q'))
		{
			m_request = ScreenRequest::Title;
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
	}
	else if (m_gameManager.IsVictory())
	{
		DrawVictory(renderer);
	}
	else if (m_isPaused)
	{
		DrawPause(renderer);
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

	if (m_gameManager.IsGameOver())
	{
		SetConsoleTitleA("GAME OVER : R pour recommencer, Echap pour le menu");
	}
	else if (m_isPaused)
	{
		SetConsoleTitleA(PAUSE_TEXT.data());
	}
	else if (m_gameManager.IsVictory())
	{
		SetConsoleTitleA(("VICTOIRE ! Score : " + std::to_string(m_gameManager.getScore()) + " : R pour rejouer").c_str());
	}
	else
	{
		SetConsoleTitleA(("Missiles actifs : " + std::to_string(activeMissiles)).c_str());
	}

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
