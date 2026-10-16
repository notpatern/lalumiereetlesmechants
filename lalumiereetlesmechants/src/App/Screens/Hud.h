#pragma once
#include "HudBar.h"
#include "ScreenText.h"
#include "../Gameplay/Experience.h"
#include "../Gameplay/PlayArea.h"
#include "../Manager/GameManager.h"
#include "../Player/Player.h"
#include <vector>
#include <windows.h>

class Hud
{
public:
	void Update(float inDeltaTime);
	void AddTexts(std::vector<ScreenText>& outTexts, const GameManager& inGameManager) const;

private:
	static constexpr int SCORE_ROW = PlayArea::HEIGHT;
	static constexpr int LIFE_ROW = PlayArea::HEIGHT;
	static constexpr int XP_ROW = PlayArea::HEIGHT + 1;

	static constexpr HudBar LIFE_BAR{ .segmentWidth = 6, .gap = 1, .filledColor = 0xC0, .emptyColor = 0x80 };
	static constexpr int LOW_LIFE_HEALTH = 1;
	static constexpr WORD LOW_LIFE_BLINK_COLOR = 0x40;
	static constexpr float LOW_LIFE_BLINK_PERIOD = 0.4f;

	static constexpr HudBar XP_BAR{ .segmentWidth = 1, .gap = 0, .filledColor = 0xE0, .emptyColor = 0x80 };
	static constexpr int XP_BAR_CELLS = 20;
	static constexpr WORD XP_TEXT_COLOR = 0x0E;

	static constexpr int WAVE_LABEL_COLUMN = 68;
	static constexpr int WAVE_LABEL_ROW = PlayArea::HEIGHT;

	float m_elapsed = 0.f;

	void AddLifeBar(std::vector<ScreenText>& outTexts, const Player& inPlayer) const;
	static void AddXpBar(std::vector<ScreenText>& outTexts, const Experience& inExperience);
};
