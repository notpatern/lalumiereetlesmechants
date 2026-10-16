#include "Menu.h"
#include <string>
#include <windows.h>

Menu::Menu(std::initializer_list<std::string_view> inOptions) : m_options(inOptions)
{
}

int Menu::Update(const InputManager& inInput)
{
	const int count = static_cast<int>(m_options.size());
	if (count == 0)
	{
		return NO_CHOICE;
	}

	if (inInput.WasJustPressed(VK_UP) || inInput.WasJustPressed('Z') || inInput.WasJustPressed('W'))
	{
		m_selected = (m_selected + count - 1) % count;
	}
	if (inInput.WasJustPressed(VK_DOWN) || inInput.WasJustPressed('S'))
	{
		m_selected = (m_selected + 1) % count;
	}

	if (inInput.WasJustPressed(VK_SPACE) || inInput.WasJustPressed(VK_RETURN))
	{
		return m_selected;
	}

	return NO_CHOICE;
}

void Menu::AddTexts(std::vector<ScreenText>& outTexts, int inFirstRow) const
{
	for (int i = 0; i < static_cast<int>(m_options.size()); ++i)
	{
		const std::string line = (i == m_selected ? "> " : "  ") + std::string(m_options[i]) + (i == m_selected ? " <" : "  ");
		outTexts.push_back(MakeCenteredText(line, inFirstRow + i * 2));
	}
}
