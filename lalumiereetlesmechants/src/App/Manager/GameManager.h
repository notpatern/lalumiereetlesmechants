#pragma once
#include "../Entites/Missile.h"
#include "../../Utilities/Pool.h"


class GameManager
{
public:
	GameManager();

	Pool<Missile> m_missilesPool;

	void Update(float deltaTime);
};

