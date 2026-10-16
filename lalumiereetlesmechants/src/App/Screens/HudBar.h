#pragma once
#include "ScreenText.h"
#include "../../Utilities/Vector2.h"
#include <vector>
#include <windows.h>

struct HudBar
{
	int segmentWidth = 1;
	int gap = 0;
	WORD filledColor = 0xF0;
	WORD emptyColor = 0x80;

	constexpr int getWidth(int segmentCount) const { return segmentCount * segmentWidth + (segmentCount - 1) * gap; }
	void AddTexts(std::vector<ScreenText>& texts, const Utility::Vector2<int>& position, int filledCount, int segmentCount) const;
};
