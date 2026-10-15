#pragma once
#include "../Manager/InputManager.h"
#include "ScreenText.h"
#include <initializer_list>
#include <string_view>
#include <vector>
#include <windows.h>

class Menu
{
public:
	static constexpr int NO_CHOICE = -1;

	explicit Menu(std::initializer_list<std::string_view> options);

	int Update(const InputManager& input);

	void AddTexts(std::vector<ScreenText>& texts, int firstRow) const;

private:
	std::vector<std::string_view> m_options;
	int m_selected = 0;
};
