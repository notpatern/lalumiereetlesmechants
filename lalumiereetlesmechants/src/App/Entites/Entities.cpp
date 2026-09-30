#include "Entities.h"

Entity::Entities::Entities() : m_position{}, m_sprite{}
{
}

Entity::Entities::Entities(Utilities::Vector2<int>* position, Sprite* sprite) : m_position{ position }, m_sprite{ sprite }
{

}

Entity::Entities::~Entities()
{
	delete m_position;
	delete m_sprite;
}
