#pragma once
#include "../Gameplay/PlayArea.h"
#include "../../Utilities/Vector2.h"
#include <string>
#include <string_view>
#include <windows.h>

// Un texte qu'un ecran veut afficher : le Renderer l'ecrit a cette case, avec cette couleur
struct ScreenText
{
	std::string text;
	Utility::Vector2<int> position;
	WORD color = 0x0F;
};

inline ScreenText MakeCenteredText(std::string_view inText, int inRow, WORD inColor = 0x0F)
{
	return { std::string(inText), { (PlayArea::WIDTH - static_cast<int>(inText.size())) / 2, inRow }, inColor };
}
