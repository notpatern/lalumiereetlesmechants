#include "Hud.h"

#include <cmath>
#include <string>

void Hud::Update(float deltaTime)
{
	m_elapsed += deltaTime;
}

void Hud::AddTexts(std::vector<ScreenText>& texts, const GameManager& gameManager) const
{
	texts.push_back({ "Score : " + std::to_string(gameManager.getScore()), { 0, SCORE_ROW } });
	AddLifeBar(texts, gameManager.getPlayer());
	AddXpBar(texts, gameManager.getPlayer().getExperience());
	texts.push_back({ "Vague " + std::to_string(gameManager.getWaveNumber()) + "/" + std::to_string(gameManager.getWaveCount()), { WAVE_LABEL_COLUMN, WAVE_LABEL_ROW } });
}

void Hud::AddLifeBar(std::vector<ScreenText>& texts, const Player& player) const
{
	const int maxHealth = player.getMaxHealth();

	HudBar bar = LIFE_BAR;
	if (player.getHealth() == 1 && std::fmod(m_elapsed, LOW_LIFE_BLINK_PERIOD) > LOW_LIFE_BLINK_PERIOD / 2.f)
	{
		bar.filledColor = LOW_LIFE_BLINK_COLOR;
	}

	const int column = (PlayArea::WIDTH - bar.getWidth(maxHealth)) / 2;
	bar.AddTexts(texts, { column, LIFE_ROW }, player.getHealth(), maxHealth);
}

void Hud::AddXpBar(std::vector<ScreenText>& texts, const Experience& experience)
{
	const int filled = experience.IsMaxLevel() ? XP_BAR_CELLS : experience.getXpInLevel() * XP_BAR_CELLS / experience.getXpForNextLevel();
	const std::string label = experience.IsMaxLevel() ? "Niv MAX " : "Niv " + std::to_string(experience.getLevel()) + " ";
	const int column = (PlayArea::WIDTH - XP_BAR.getWidth(XP_BAR_CELLS)) / 2;

	texts.push_back({ label, { column - static_cast<int>(label.size()), XP_ROW }, XP_TEXT_COLOR });
	XP_BAR.AddTexts(texts, { column, XP_ROW }, filled, XP_BAR_CELLS);
}
