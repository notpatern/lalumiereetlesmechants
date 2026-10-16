#include "App.h"
#include <windows.h>
#include <timeapi.h>
#include <iostream>
#include <memory>

#include "Screens/CreditsScreen.h"
#include "Screens/IScreen.h"
#include "Screens/GameScreen.h"
#include "Screens/TitleScreen.h"

#pragma comment(lib, "winmm.lib")

App::App(Utility::Timer* timer) : m_timer(timer), m_consoleHandle(GetStdHandle(STD_OUTPUT_HANDLE))
{
	timeBeginPeriod(TIMER_RESOLUTION_MS);
	const LONG_PTR fixedSizeStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	setConsoleWindowStyle(GWL_STYLE, fixedSizeStyle);
	SetupConsole();

	if (GetConsoleScreenBufferInfo(m_consoleHandle, &m_consoleInfo)) {
		m_windowSize.x = m_consoleInfo.srWindow.Right - m_consoleInfo.srWindow.Left + 1;
		m_windowSize.y = m_consoleInfo.srWindow.Bottom - m_consoleInfo.srWindow.Top + 1;
	}

	m_world.SetArraySizes(Utility::Vector2<int>(80, 40));
}

App::~App()
{
	timeEndPeriod(TIMER_RESOLUTION_MS);
	delete m_timer;
}

void App::Run()
{
	static InputManager input;
	static std::unique_ptr<IScreen> screen = std::make_unique<TitleScreen>();

	const float dt = m_timer->getElapsedSeconds(true);
	input.Update();
	screen->Update(dt, input);

	switch (screen->getRequest())
	{
	case ScreenRequest::NewGame:
		screen = std::make_unique<GameScreen>();
		break;
	case ScreenRequest::Title:
		screen = std::make_unique<TitleScreen>();
		break;
	case ScreenRequest::Credits:
		screen = std::make_unique<CreditsScreen>();
		break;
	case ScreenRequest::Quit:
		m_isRunning = false;
		break;
	case ScreenRequest::None:
		break;
	}

	KeepConsoleSize();
	Renderer::getInstance().Render();

	Sleep(16);
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

void App::SetupConsole()
{
	const SMALL_RECT tinyWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_consoleHandle, TRUE, &tinyWindow);
	SetConsoleScreenBufferSize(m_consoleHandle, { Renderer::SCREEN_WIDTH, Renderer::SCREEN_HEIGHT });
	const SMALL_RECT gameWindow = { 0, 0, Renderer::SCREEN_WIDTH - 1, Renderer::SCREEN_HEIGHT - 1 };
	if (!SetConsoleWindowInfo(m_consoleHandle, TRUE, &gameWindow))
	{
		const COORD largest = GetLargestConsoleWindowSize(m_consoleHandle);
		const SHORT width = largest.X < Renderer::SCREEN_WIDTH ? largest.X : Renderer::SCREEN_WIDTH;
		const SHORT height = largest.Y < Renderer::SCREEN_HEIGHT ? largest.Y : Renderer::SCREEN_HEIGHT;
		const SMALL_RECT fitWindow = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
		SetConsoleWindowInfo(m_consoleHandle, TRUE, &fitWindow);
	}

	const HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD inputMode = 0;
	GetConsoleMode(inputHandle, &inputMode);
	SetConsoleMode(inputHandle, (inputMode | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);

	CONSOLE_CURSOR_INFO cursorInfo{ 1, FALSE };
	SetConsoleCursorInfo(m_consoleHandle, &cursorInfo);

	SetConsoleTitleA("MAUVAIS CARACTERES");
}

void App::KeepConsoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFO info{};
	if (!GetConsoleScreenBufferInfo(m_consoleHandle, &info))
	{
		return;
	}

	const bool isSameBuffer = info.dwSize.X == m_lastBufferSize.X && info.dwSize.Y == m_lastBufferSize.Y;
	const bool isSameWindow = info.srWindow.Left == m_lastWindow.Left && info.srWindow.Top == m_lastWindow.Top
		&& info.srWindow.Right == m_lastWindow.Right && info.srWindow.Bottom == m_lastWindow.Bottom;
	if (isSameBuffer && isSameWindow)
	{
		return;
	}

	DWORD written = 0;
	const DWORD cellCount = static_cast<DWORD>(info.dwSize.X) * static_cast<DWORD>(info.dwSize.Y);
	FillConsoleOutputCharacterA(m_consoleHandle, ' ', cellCount, { 0, 0 }, &written);
	FillConsoleOutputAttribute(m_consoleHandle, 0, cellCount, { 0, 0 }, &written);
	SetupConsole();

	if (GetConsoleScreenBufferInfo(m_consoleHandle, &info))
	{
		m_lastBufferSize = info.dwSize;
		m_lastWindow = info.srWindow;
	}
}
