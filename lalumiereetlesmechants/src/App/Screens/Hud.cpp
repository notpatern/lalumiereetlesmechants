#include "Hud.h"
#include <cmath>
#include <string>

void Hud::Update(float inDeltaTime)
{
	m_elapsed += inDeltaTime;
}

void Hud::AddTexts(std::vector<ScreenText>& outTexts, const GameManager& inGameManager) const
{
	outTexts.push_back({ "Score : " + std::to_string(inGameManager.getScore()), { 0, SCORE_ROW } });
	AddLifeBar(outTexts, inGameManager.getPlayer());
	AddXpBar(outTexts, inGameManager.getPlayer().getExperience());
	outTexts.push_back({ "Vague " + std::to_string(inGameManager.getWaveNumber()) + "/" + std::to_string(inGameManager.getWaveCount()), { WAVE_LABEL_COLUMN, WAVE_LABEL_ROW } });
}

void Hud::AddLifeBar(std::vector<ScreenText>& outTexts, const Player& inPlayer) const
{
	const int maxHealth = inPlayer.getMaxHealth();

	HudBar bar = LIFE_BAR;
	if (inPlayer.getHealth() == LOW_LIFE_HEALTH && std::fmod(m_elapsed, LOW_LIFE_BLINK_PERIOD) > LOW_LIFE_BLINK_PERIOD / 2.f)
	{
		bar.filledColor = LOW_LIFE_BLINK_COLOR;
	}

	const int column = (PlayArea::WIDTH - bar.ComputeWidth(maxHealth)) / 2;
	bar.AddTexts(outTexts, { column, LIFE_ROW }, inPlayer.getHealth(), maxHealth);
}

void Hud::AddXpBar(std::vector<ScreenText>& outTexts, const Experience& inExperience)
{
	const int filled = inExperience.IsMaxLevel() ? XP_BAR_CELLS : inExperience.getXpInLevel() * XP_BAR_CELLS / inExperience.getXpForNextLevel();
	const std::string label = inExperience.IsMaxLevel() ? "Niv MAX " : "Niv " + std::to_string(inExperience.getLevel()) + " ";
	const int column = (PlayArea::WIDTH - XP_BAR.ComputeWidth(XP_BAR_CELLS)) / 2;

	outTexts.push_back({ label, { column - static_cast<int>(label.size()), XP_ROW }, XP_TEXT_COLOR });
	XP_BAR.AddTexts(outTexts, { column, XP_ROW }, filled, XP_BAR_CELLS);
}
