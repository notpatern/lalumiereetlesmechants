#pragma once
#include "../Manager/InputManager.h"
#include "../Render/Renderer.h"
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

	void Draw(Renderer& renderer, int firstRow) const;
	void DrawDebug(HANDLE console, int firstRow) const;

private:
	std::vector<std::string_view> m_options;
	int m_selected = 0;
};
