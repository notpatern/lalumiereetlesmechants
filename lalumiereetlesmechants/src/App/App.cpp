#include "App.h"
#include <windows.h>
#include <iostream>

App::App(Utility::Timer* timer) : m_timer(timer), m_consoleHandle(GetStdHandle(STD_OUTPUT_HANDLE)), m_renderer(&m_world)
{
	LONG_PTR new_style =  WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL;
    setConsoleWindowStyle(GWL_STYLE,new_style);
	
	if (GetConsoleScreenBufferInfo(m_consoleHandle, &m_consoleInfo)) {
		m_windowSize.x = m_consoleInfo.srWindow.Right - m_consoleInfo.srWindow.Left + 1;
		m_windowSize.y = m_consoleInfo.srWindow.Bottom - m_consoleInfo.srWindow.Top + 1;
	}

	m_world.SetArraySizes(m_windowSize);
}

App::~App()
{
	delete m_timer;
}

void App::Run() {
	m_renderer.Render();
}

LONG_PTR App::setConsoleWindowStyle(INT n_index, LONG_PTR new_style)
{
	SetLastError(NO_ERROR);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    std::cout << "\033[38;2;20;20;20mGris 20\n";
    std::cout << "\033[38;2;50;50;50mGris 50\n";
    std::cout << "\033[38;2;80;80;80mGris 80\n";
    std::cout << "\033[38;2;110;110;110mGris 110\n";
    std::cout << "\033[38;2;140;140;140mGris 140\n";
    std::cout << "\033[38;2;170;170;170mGris 170\n";
    std::cout << "\033[38;2;200;200;200mGris 200\n";
    std::cout << "\033[38;2;230;230;230mGris 230\n";
    std::cout << "\033[38;2;255;255;255mBlanc\n";

    std::cout << "\033[0m";

	HWND hwnd_console = GetConsoleWindow();
	LONG_PTR style_ptr = SetWindowLongPtr(hwnd_console, n_index, new_style);
	SetWindowPos(hwnd_console, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);

	ShowWindow(hwnd_console, SW_SHOW);

	return style_ptr;
}
