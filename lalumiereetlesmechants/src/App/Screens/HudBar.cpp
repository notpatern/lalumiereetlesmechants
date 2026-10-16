#include "HudBar.h"
#include <string>

void HudBar::AddTexts(std::vector<ScreenText>& texts, const Utility::Vector2<int>& position, int filledCount, int segmentCount) const
{
	for (int i = 0; i < segmentCount; ++i)
	{
		const WORD color = i < filledCount ? filledColor : emptyColor;
		texts.push_back({ std::string(segmentWidth, ' '), { position.x + i * (segmentWidth + gap), position.y }, color });
	}
}
