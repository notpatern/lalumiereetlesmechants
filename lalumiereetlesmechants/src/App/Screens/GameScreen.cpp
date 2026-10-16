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
		m_gameManager.Update(deltaTime, input);
		m_endElapsed += deltaTime;
		if (m_endElapsed < END_INPUT_DELAY)
		{
			return;
		}

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

#ifdef _DEBUG
	if (input.WasJustPressed(VK_F2))
	{
		m_gameManager.SkipWave();
	}
#endif

	m_gameManager.Update(deltaTime, input);
	m_hud.Update(deltaTime);
}

std::vector<ScreenText> GameScreen::getTexts() const
{
	std::vector<ScreenText> texts;

	m_hud.AddTexts(texts, m_gameManager);

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

	const Utility::Vector2<int> cameraOffset = getCameraOffset();

	DWORD written = 0;
	auto draw = [&](const Utility::Vector2<float>& position, char character)
	{
		const Utility::Vector2<int> worldCell = Collision::ToCell(position);
		const Utility::Vector2<int> cellPosition{ worldCell.x + cameraOffset.x, worldCell.y + cameraOffset.y };
		if (!PlayArea::Bounds().Contains(cellPosition))
		{
			return;
		}
		const COORD cell{static_cast<SHORT>(cellPosition.x), static_cast<SHORT>(cellPosition.y)};
		FillConsoleOutputCharacterA(console, character, 1, cell, &written);
		FillConsoleOutputAttribute(console, 0x0F, 1, cell, &written);
	};

	if (!player.IsDead() && player.IsVisible())
	{
		draw(player.getPosition(), '@');
	}
	int activeMissiles = 0;
	m_gameManager.getMissiles().ForEachActive([&](const Missile& missile)
	{
		const Utility::Vector2<int> size = missile.getHitboxSize();
		const char character = MissileDebugCharacter(missile.getType());
		for (int y = 0; y < size.y; ++y)
		{
			for (int x = 0; x < size.x; ++x)
			{
				draw({ missile.getPosition().x + x, missile.getPosition().y + y }, character);
			}
		}
		++activeMissiles;
	});

	SetConsoleTitleA(("Missiles actifs : " + std::to_string(activeMissiles)).c_str());

	m_gameManager.getParticles().ForEachActive([&](const Particle& particle)
	{
		const SpriteUtils::SpriteType type = particle.getSpriteType();
		draw(particle.getPosition(), type == SpriteUtils::SpriteType::LevelUpSpark ? '+' : (type == SpriteUtils::SpriteType::Smoke ? ',' : '*'));
	});

	m_gameManager.getXpOrbs().ForEachActive([&](const XpOrb& orb)
	{
		draw(orb.getPosition(), '+');
	});

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

		draw(enemy.getPosition(), enemy.IsFlashing() ? '*' : (enemy.IsTelegraphing() ? '!' : EnemyDebugCharacter(enemy.getType())));
	});

	DebugConsole::WriteTexts(console, getTexts());
}

char GameScreen::EnemyDebugCharacter(EnemyType type)
{
	switch (type)
	{
	case EnemyType::Turret:
		return 'T';
	case EnemyType::Charger:
		return 'B';
	case EnemyType::Swarm:
		return 'N';
	case EnemyType::Hunter:
	default:
		return 'C';
	}
}

char GameScreen::MissileDebugCharacter(MissileType type)
{
	switch (type)
	{
	case MissileType::EnemyShot:
		return 'o';
	case MissileType::BigEnemyShot:
		return 'O';
	case MissileType::PlayerShot:
	default:
		return '|';
	}
}
