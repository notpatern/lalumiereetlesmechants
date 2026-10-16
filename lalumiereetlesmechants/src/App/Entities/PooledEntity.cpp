#include "PooledEntity.h"

void PooledEntity::Activate()
{
	m_isActive = true;
	SetInRenderQueue(true);
}

void PooledEntity::Deactivate()
{
	m_isActive = false;
	SetInRenderQueue(false);
}
