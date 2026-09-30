#pragma once
#include "Entities.h"
#include "../Render/Lightable.h"

class Enemy : public Entity::Entities, ILightable
{
public:
	Enemy();
	Enemy(const Utilities::Vector2<int>& position, Sprite* sprite);
	~Enemy();

	void Update() override;
};

