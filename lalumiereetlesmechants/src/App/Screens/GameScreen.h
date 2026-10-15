#pragma once
#include "IScreen.h"
#include "../Manager/GameManager.h"
#include "../Manager/InputManager.h"
#include "../Render/Renderer.h"
#include <string_view>
#include <windows.h>

class GameScreen : public IScreen
{
public:
	void Update(float deltaTime, const InputManager& input) override;
	void Draw(Renderer& renderer) const override;
	void DrawDebug(HANDLE console) const override;
	ScreenRequest getRequest() const override { return m_request; }

private:
	static constexpr std::string_view GAME_OVER_TEXT = "Defaite";
	static constexpr std::string_view VICTORY_TEXT = "Victoire";
	static constexpr std::string_view RESTART_TEXT = "R pour recommencer";
	static constexpr std::string_view PAUSE_TEXT = "Pause : Echap pour reprendre, Q pour le menu";

	GameManager m_gameManager;

	ScreenRequest m_request = ScreenRequest::None;

	bool m_isPaused = false;

	static void DrawGameOver(Renderer& renderer);
	void DrawVictory(Renderer& renderer) const;
	static void DrawPause(Renderer& renderer);
	static int CenteredColumn(std::string_view text);
};
