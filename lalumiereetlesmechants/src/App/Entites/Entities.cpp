#include "Entities.h"

#include "../Render/Renderer.h"

Entity::Entity() : Sprite()
{
	SetSpritePosition(&m_position);
}

Entity::Entity(const Utility::Vector2<float>& position) : m_position( position )
{
	SetSpritePosition(&m_position);
}

Entity::~Entity()
{
}

void Entity::SetInRenderQueue(bool shouldBeInQueue)
{
	if (shouldBeInQueue == m_isInRenderQueue)
	{
		return;
	}

	if (shouldBeInQueue)
	{
		Renderer::getInstance().AddToRenderQueue(this);
	}
	else
	{
		Renderer::getInstance().RemoveFromRenderQueue(this);
	}
	m_isInRenderQueue = shouldBeInQueue;
}
