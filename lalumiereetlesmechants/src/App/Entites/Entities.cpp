#include "Entities.h"

Entity::Entity::Entity() : Sprite()
{
	
}

Entity::Entity::Entity(const Utility::Vector2<float>& position) : m_position( position )
{

}

Entity::Entity::~Entity()
{
}
