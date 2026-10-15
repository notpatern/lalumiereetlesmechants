#include "Renderer.h"
#include <iostream>
#include <Windows.h>

Renderer::Renderer()
{
}

Renderer::Renderer(World* world) : m_world(world)
{
}

Renderer::~Renderer()
{
}


void Renderer::Render()
{
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwBufferSize = { SCREEN_WIDTH,SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

	ReadConsoleOutput(hOutput, (CHAR_INFO*)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);

	for (int x = 0; x < SCREEN_WIDTH; ++x)
	{
		for (int y = 0; y < SCREEN_HEIGHT; ++y)
		{
			if (CanAccessMap(*m_world, Utility::Vector2<int>(x, y)))
			{
				int lightValue = (int)GetLightMapValue(*m_world, { x, y });
				char worldChar = (char)GetWorldMapValue(*m_world, { x, y });

				buffer[y][x].Char.AsciiChar = worldChar;
				buffer[y][x].Attributes = 0xFFFFFF;
			}
			else
			{
				buffer[y][x].Char.AsciiChar = ' ';
				buffer[y][x].Attributes = 0;
			}
		}
	}

	WriteConsoleOutput(hOutput, (CHAR_INFO*)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);

	for (Sprite* currentSprite : m_spriteRenderQueue) {
		RenderSprite(currentSprite);
	}
}

void Renderer::RenderSprite(Sprite* sprite) {
//	for (int y = 0; y < m_size.y; y++)
//	{
//		for (int x = 0; x < m_size.x; x++)
//		{
//			if (CanAccessMap(world,
//				Utility::Vector2<int>(m_position.x + x, m_position.y + y))
//				)
//			{
//				SetMapValue(world, Utility::Vector2<int>(
//					m_position.x + x, m_position.y + y),
//					m_sprite[y][x],
//					world.getWorldMap());
//			}
//		}
//	}
}

void Renderer::AddToRenderQueue(Sprite* sprite)
{
	m_spriteRenderQueue.push_back(sprite);
}

void Renderer::AddToRenderQueue(IScreen* screen)
{
	m_screenRenderQueue.push_back(screen);
}

void Renderer::RemoveFromRenderQueue(Sprite* sprite)
{
	m_spriteRenderQueue.erase(std::remove(m_spriteRenderQueue.begin(), m_spriteRenderQueue.end(), sprite), m_spriteRenderQueue.end());
}

void Renderer::RemoveFromRenderQueue(IScreen* screen)
{
	m_screenRenderQueue.erase(std::remove(m_screenRenderQueue.begin(), m_screenRenderQueue.end(), screen), m_screenRenderQueue.end());
}
