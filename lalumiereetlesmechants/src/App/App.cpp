#include "App.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <memory>

#include "Gameplay/Collision.h"
#include "Manager/GameManager.h"
#include "Screens/GameScreen.h"
#include "Player/Player.h"

App::App(Utility::Timer* timer) : m_timer(timer), m_consoleHandle(GetStdHandle(STD_OUTPUT_HANDLE)), m_renderer(&m_world)
{
	LONG_PTR new_style =  WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL;
    setConsoleWindowStyle(GWL_STYLE,new_style);

	if (GetConsoleScreenBufferInfo(m_consoleHandle, &m_consoleInfo)) {
		m_windowSize.x = m_consoleInfo.srWindow.Right - m_consoleInfo.srWindow.Left + 1;
		m_windowSize.y = m_consoleInfo.srWindow.Bottom - m_consoleInfo.srWindow.Top + 1;
	}

	m_world.SetArraySizes(Utility::Vector2<int>(80, 40));
}

App::~App()
{
	delete m_timer;
}

void App::Run() {
	// --- TEST
	static InputManager input;
	static std::unique_ptr<GameScreen> gameScreen = std::make_unique<GameScreen>();

	const float dt = m_timer->getElapsedSeconds(true);
	input.Update();
	gameScreen->Update(dt, input);

	if (gameScreen->WantsRestart())
	{
		gameScreen = std::make_unique<GameScreen>();
	}

	const GameManager& gameManager = gameScreen->getGameManager();
	const Player& player = gameManager.getPlayer();

	//m_renderer.Render();

	DWORD written = 0;
	auto draw = [&](const Utility::Vector2<float>& position, char character)
	{
		const Utility::Vector2<int> cellPosition = Collision::ToCell(position);
		const COORD cell{static_cast<SHORT>(cellPosition.x), static_cast<SHORT>(cellPosition.y)};
		FillConsoleOutputCharacterA(m_consoleHandle, character, 1, cell, &written);
		FillConsoleOutputAttribute(m_consoleHandle, 0x0F, 1, cell, &written);
	};

	draw(player.getPosition(), '@');
	int activeMissiles = 0;
	gameManager.getMissiles().ForEachActive([&](const Missile& missile)
	{
		draw(missile.getCurrentPosition(), '|');
		++activeMissiles;
	});

	if (gameManager.IsGameOver())
	{
		SetConsoleTitleA("GAME OVER : R pour recommencer");
	}
	else
	{
		SetConsoleTitleA(("Missiles actifs : " + std::to_string(activeMissiles)).c_str());
	}

	gameManager.getEnemies().ForEachActive([&](const Enemy& enemy)
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

	Sleep(16);
	// --- FIN DU TEST ---
}

LONG_PTR App::setConsoleWindowStyle(INT n_index, LONG_PTR new_style)
{
	SetLastError(NO_ERROR);
	HWND hwnd_console = GetConsoleWindow();
	LONG_PTR style_ptr = SetWindowLongPtr(hwnd_console, n_index, new_style);
	SetWindowPos(hwnd_console, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);

	ShowWindow(hwnd_console, SW_SHOW);

	return style_ptr;
}
