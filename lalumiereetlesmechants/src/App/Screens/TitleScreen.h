#pragma once
#include "IScreen.h"
#include "Menu.h"
#include "ScreenText.h"
#include "../Manager/InputManager.h"
#include <string_view>
#include <vector>
#include <windows.h>

class TitleScreen : public IScreen
{
public:
	void Update(float inDeltaTime, const InputManager& inInput) override;
	void DrawDebug(HANDLE console) const override;

	std::vector<ScreenText> getTexts() const override;
	ScreenRequest getRequest() const override { return m_request; }

private:
	static constexpr std::string_view TITLE_TEXT = "Mauvais Character";

	static constexpr int PLAY_CHOICE = 0;
	static constexpr int CREDITS_CHOICE = 1;
	static constexpr int QUIT_CHOICE = 2;

	Menu m_menu{ "Jouer", "Credits", "Quitter" };
	ScreenRequest m_request = ScreenRequest::None;
};
