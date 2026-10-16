#pragma once
#include "ScreenText.h"
#include "../Manager/InputManager.h"
#include <initializer_list>
#include <string_view>
#include <vector>

class Menu
{
public:
	static constexpr int NO_CHOICE = -1;

	explicit Menu(std::initializer_list<std::string_view> inOptions);

	int Update(const InputManager& inInput);

	void AddTexts(std::vector<ScreenText>& outTexts, int inFirstRow) const;

private:
	std::vector<std::string_view> m_options;
	int m_selected = 0;
};
