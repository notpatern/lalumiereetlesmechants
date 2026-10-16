#include "HudBar.h"
#include <string>

void HudBar::AddTexts(std::vector<ScreenText>& outTexts, const Utility::Vector2<int>& inPosition, int inFilledCount, int inSegmentCount) const
{
	for (int i = 0; i < inSegmentCount; ++i)
	{
		const WORD color = i < inFilledCount ? filledColor : emptyColor;
		outTexts.push_back({ std::string(segmentWidth, ' '), { inPosition.x + i * (segmentWidth + gap), inPosition.y }, color });
	}
}
