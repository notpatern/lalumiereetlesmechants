#pragma once
#include "IScreen.h"
#include "Menu.h"
#include "../Gameplay/EnemyArchetype.h"
#include "../Manager/GameManager.h"
#include "../Manager/InputManager.h"
#include <string_view>
#include <windows.h>
#include <vector>

#include "Hud.h"

class GameScreen : public IScreen
{
public:
	void Update(float deltaTime, const InputManager& input) override;
	std::vector<ScreenText> getTexts() const override;
	void DrawDebug(HANDLE console) const override;
	ScreenRequest getRequest() const override { return m_request; }

private:
	static constexpr std::string_view GAME_OVER_TEXT = "Defaite";
	static constexpr std::string_view VICTORY_TEXT = "Victoire";
	static constexpr std::string_view PAUSE_TEXT = "Pause";

	GameManager m_gameManager;
	Hud m_hud;

	ScreenRequest m_request = ScreenRequest::None;

	bool m_isPaused = false;

	static constexpr int PAUSE_RESUME_CHOICE = 0;
	static constexpr int PAUSE_RESTART_CHOICE = 1;
	static constexpr int PAUSE_MENU_CHOICE = 2;
	Menu m_pauseMenu{ "Reprendre", "Recommencer", "Menu" };

	static constexpr int END_RESTART_CHOICE = 0;
	static constexpr int END_MENU_CHOICE = 1;
	Menu m_endMenu{ "Recommencer", "Menu" };

	static char EnemyDebugCharacter(EnemyType type);
};
