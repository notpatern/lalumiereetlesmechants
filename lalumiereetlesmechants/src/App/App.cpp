#include "App.h"
#include <windows.h>
#include <iostream>
#include <string>

#include "Manager/GameManager.h"
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
	m_renderer.Render();
	// TEST
	static InputManager input;
	static GameManager gameManager;
	static Player player{ Utility::Vector2<float>(10.f, 10.f), gameManager.m_missilesPool };
	static COORD previousCell{ -1, -1 };

	const float dt = m_timer->getElapsedSeconds(true);
	gameManager.Update(dt);
	input.Update();
	player.Update(dt, input);

	const Utility::Vector2<float> position = player.getPosition();
	const COORD cell{ static_cast<SHORT>(position.x), static_cast<SHORT>(position.y) };

	if (cell.X != previousCell.X || cell.Y != previousCell.Y)
	{
		DWORD written = 0;
		FillConsoleOutputCharacterA(m_consoleHandle, ' ', 1, previousCell, &written);
		FillConsoleOutputCharacterA(m_consoleHandle, '@', 1, cell, &written);
		previousCell = cell;

		const std::string title = "x = " + std::to_string(position.x) + "   y = " + std::to_string(position.y);
		SetConsoleTitleA(title.c_str());
	}
	// FON TEST

	//m_renderer.Render();
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
