#include "GameScreen.h"
#include "../Gameplay/PlayArea.h"

void GameScreen::Update(float deltaTime, const InputManager& input)
{
	m_gameManager.Update(deltaTime, input);

	m_wantsRestart = m_gameManager.IsGameOver() && input.WasJustPressed('R');
}

void GameScreen::Draw(Renderer& renderer) const
{
	//la jvais demander au renderer de dessiner les ennemis, les missiles et le joueur (sivisible), avec leurs sprites

	if (m_gameManager.IsGameOver())
	{
		DrawGameOver(renderer);
	}
}

void GameScreen::DrawGameOver(Renderer& renderer)
{
	//la jvais ecrire le game over au milieu de la zone de jeu, et la touche R pour recommencer en dessous
}

int GameScreen::CenteredColumn(std::string_view text)
{
	return (PlayArea::WIDTH - static_cast<int>(text.size())) / 2;
}
