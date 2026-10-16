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

	constexpr int ComputeWidth(int inSegmentCount) const
	{
		return inSegmentCount * segmentWidth + (inSegmentCount - 1) * gap;
	}

	void AddTexts(std::vector<ScreenText>& outTexts, const Utility::Vector2<int>& inPosition, int inFilledCount, int inSegmentCount) const;
};
