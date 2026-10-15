#include "Menu.h"
#include "DebugConsole.h"
#include <string>

Menu::Menu(std::initializer_list<std::string_view> options) : m_options(options)
{
}

int Menu::Update(const InputManager& input)
{
	const int count = static_cast<int>(m_options.size());
	if (count == 0)
	{
		return NO_CHOICE;
	}

	if (input.WasJustPressed(VK_UP) || input.WasJustPressed('Z') || input.WasJustPressed('W'))
	{
		m_selected = (m_selected + count - 1) % count;
	}
	if (input.WasJustPressed(VK_DOWN) || input.WasJustPressed('S'))
	{
		m_selected = (m_selected + 1) % count;
	}

	if (input.WasJustPressed(VK_SPACE) || input.WasJustPressed(VK_RETURN))
	{
		return m_selected;
	}

	return NO_CHOICE;
}

void Menu::Draw(Renderer& renderer, int firstRow) const
{
	//TODO ecrire les options
}

void Menu::DrawDebug(HANDLE console, int firstRow) const
{
	for (int i = 0; i < static_cast<int>(m_options.size()); ++i)
	{
		const std::string line = (i == m_selected ? "> " : "  ") + std::string(m_options[i]) + (i == m_selected ? " <" : "  ");
		DebugConsole::WriteCentered(console, line, firstRow + i * 2);
	}
}
