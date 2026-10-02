#pragma once
#include "../Entites/Missile.h"
#include "../../Utilities/Pool.h"


class GameManager
{
public:
	GameManager();

	Utility::Pool<Missile> m_missilesPool;

	void Update(float deltaTime);
};

