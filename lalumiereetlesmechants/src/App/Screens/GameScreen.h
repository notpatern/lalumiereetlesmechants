#pragma once
#include "../Manager/GameManager.h"
#include "../Manager/InputManager.h"
#include "../Render/Renderer.h"
#include <string_view>

class GameScreen
{
public:
	void Update(float deltaTime, const InputManager& input);
	void Draw(Renderer& renderer) const;
	bool WantsRestart() const { return m_wantsRestart; }
	const GameManager& getGameManager() const { return m_gameManager; }

private:
	static constexpr std::string_view GAME_OVER_TEXT = "Game Over";
	static constexpr std::string_view VICTORY_TEXT = "Victoire";
	static constexpr std::string_view RESTART_TEXT = "R pour recommencer";

	GameManager m_gameManager;

	bool m_wantsRestart = false;

	static void DrawGameOver(Renderer& renderer);
	void DrawVictory(Renderer& renderer) const;
	static int CenteredColumn(std::string_view text);
};
