#pragma once
#include "Entities.h"
#include "../Render/Lightable.h"

class Enemy : public Entity, ILightable
{
public:
	Enemy();
	Enemy(const Utility::Vector2<int>& position, Sprite* sprite);
	~Enemy();

	void Update() override;
};

