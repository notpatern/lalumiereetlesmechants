#pragma once
#include "../../Utilities/Vector2.h"
#include "../Gameplay/PlayArea.h"
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

inline ScreenText CenteredText(std::string_view text, int row, WORD color = 0x0F)
{
	return { std::string(text), { (PlayArea::WIDTH - static_cast<int>(text.size())) / 2, row }, color };
}
