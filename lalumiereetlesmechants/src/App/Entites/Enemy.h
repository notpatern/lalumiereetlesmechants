#pragma once
#include "Entities.h"
#include "../Render/Lightable.h"

class Enemy : public Entity::Entities, ILightable
{
public:
	Enemy();
	Enemy(Utilities::Vector2<int>* position, Sprite* sprite);

};

