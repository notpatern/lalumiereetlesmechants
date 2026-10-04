#pragma once
#include "Collision.h"

namespace PlayArea
{
	inline constexpr int WIDTH = 80;
	inline constexpr int HEIGHT = 40;
	inline constexpr int OFFSCREEN_MARGIN = 5;
	inline constexpr float SPAWN_Y = -2.f;

	static_assert(SPAWN_Y >= -OFFSCREEN_MARGIN, "Les ennemis doivent apparaitre dans la marge, sinon ils sont desactives des leur apparition!!!");

	inline Collision::CellRect Bounds()
	{
		return {{0,0}, {WIDTH - 1, HEIGHT -1}};
	}

	inline Collision::CellRect ExitBounds()
	{
		return {{-OFFSCREEN_MARGIN, -OFFSCREEN_MARGIN}, {WIDTH - 1 + OFFSCREEN_MARGIN, HEIGHT -1 + OFFSCREEN_MARGIN}};
	}
}
