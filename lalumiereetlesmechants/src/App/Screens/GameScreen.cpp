#include "GameScreen.h"
#include "../Gameplay/PlayArea.h"

void GameScreen::Update(float deltaTime, const InputManager& input)
{
	m_gameManager.Update(deltaTime, input);

	m_wantsRestart = (m_gameManager.IsGameOver() || m_gameManager.IsVictory()) && input.WasJustPressed('R');
}

void GameScreen::Draw(Renderer& renderer) const
{
	//la jvais demander au renderer de dessiner les ennemis, les missiles et le joueur (sivisible), avec leurs sprites

	if (m_gameManager.IsGameOver())
	{
		DrawGameOver(renderer);
	}
	else if (m_gameManager.IsVictory())
	{
		DrawVictory(renderer);
	}
}

void GameScreen::DrawGameOver(Renderer& renderer)
{
	//la jvais ecrire le game over au milieu de la zone de jeu, et la touche R pour recommencer en dessous
}

void GameScreen::DrawVictory(Renderer& renderer) const
{
	//TODO : Render la victoire
}

int GameScreen::CenteredColumn(std::string_view text)
{
	return (PlayArea::WIDTH - static_cast<int>(text.size())) / 2;
}
