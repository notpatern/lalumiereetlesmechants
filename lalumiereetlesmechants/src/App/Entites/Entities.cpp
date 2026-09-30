#include "Entities.h"

Entity::Entities::Entities() : m_sprite()
{
	
}

Entity::Entities::Entities(const Utilities::Vector2<int>& position, Sprite* sprite) : m_position( position ), m_sprite( sprite )
{

}

Entity::Entities::~Entities()
{
	delete m_sprite;
}
