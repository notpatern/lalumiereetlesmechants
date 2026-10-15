#pragma once
#include "IScreen.h"
#include "Menu.h"
#include "../Manager/InputManager.h"
#include <string_view>
#include <windows.h>
#include <vector>

class TitleScreen : public IScreen
{
public:
	void Update(float deltaTime, const InputManager& input) override;
	std::vector<ScreenText> getTexts() const override;
	void DrawDebug(HANDLE console) const override;
	ScreenRequest getRequest() const override { return m_request; }

private:
	static constexpr std::string_view TITLE_TEXT = "La lumiere et les mechants";
	static constexpr int PLAY_CHOICE = 0;
	static constexpr int CREDITS_CHOICE = 1;
	static constexpr int QUIT_CHOICE = 2;

	Menu m_menu{ "Jouer", "Credits", "Quitter" };

	ScreenRequest m_request = ScreenRequest::None;
};
