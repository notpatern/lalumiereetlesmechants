#include "Entities.h"

Entity::Entity::Entity() : m_sprite()
{
	
}

Entity::Entity::Entity(const Utility::Vector2<int>& position, Sprite* sprite) : m_position( position ), m_sprite( sprite )
{

}

Entity::Entity::~Entity()
{
	delete m_sprite;
}
