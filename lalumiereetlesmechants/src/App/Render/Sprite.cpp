#include "Sprite.h"
#include "Renderer.h"

Sprite::Sprite(const std::vector<std::vector<char>>& sprite) : m_sprite(std::move(sprite))
{
	Renderer::getInstance().AddToRenderQueue(this);
	m_size.x = m_sprite[0].size();
}

Sprite::~Sprite()
{
}

void Sprite::Render(const World& world) {
	for (int y = 0; y < m_size.y; y++)
	{
		for (int x = 0; x < m_size.x; x++)
		{
			if (CanAccessMap(world,
				Utility::Vector2<int>(m_position.x + x, m_position.y + y))
				)
			{
				SetMapValue(world, Utility::Vector2<int>(
					m_position.x + x, m_position.y + y),
					m_sprite[y][x],
					world.getWorldMap());
			}
		}

	}
}
