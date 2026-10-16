#pragma once
#include "Hud.h"
#include "IScreen.h"
#include "Menu.h"
#include "ScreenText.h"
#include "../Gameplay/EnemyArchetype.h"
#include "../Gameplay/MissileArchetype.h"
#include "../Manager/GameManager.h"
#include "../Manager/InputManager.h"
#include "../../Utilities/Vector2.h"
#include <string_view>
#include <vector>
#include <windows.h>

class GameScreen : public IScreen
{
public:
	void Update(float inDeltaTime, const InputManager& inInput) override;
	void DrawDebug(HANDLE console) const override;

	std::vector<ScreenText> getTexts() const override;
	ScreenRequest getRequest() const override { return m_request; }
	Utility::Vector2<int> getCameraOffset() const override { return m_gameManager.getShakeOffset(); }
	bool DrawsPanel() const override { return false; }

private:
	static constexpr std::string_view GAME_OVER_TEXT = "Defaite";
	static constexpr std::string_view VICTORY_TEXT = "Victoire";
	static constexpr std::string_view PAUSE_TEXT = "Pause";

	static constexpr int PAUSE_RESUME_CHOICE = 0;
	static constexpr int PAUSE_RESTART_CHOICE = 1;
	static constexpr int PAUSE_MENU_CHOICE = 2;

	static constexpr int END_RESTART_CHOICE = 0;
	static constexpr int END_MENU_CHOICE = 1;
	static constexpr float END_INPUT_DELAY = 1.f;

	GameManager m_gameManager;
	Hud m_hud;
	ScreenRequest m_request = ScreenRequest::None;

	bool m_isPaused = false;
	Menu m_pauseMenu{ "Reprendre", "Recommencer", "Menu" };

	Menu m_endMenu{ "Recommencer", "Menu" };
	float m_endElapsed = 0.f;

	static char EnemyDebugCharacter(EnemyType type);
	static char MissileDebugCharacter(MissileType type);
};
