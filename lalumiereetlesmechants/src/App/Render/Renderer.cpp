#include "Renderer.h"
#include "../Gameplay/Collision.h"
#include <algorithm>
#include <cmath>
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
	Utility::Vector2<int> cameraOffset = { 0, 0 };
	for (IScreen* currentScreen : m_screenRenderQueue)
	{
		Utility::Vector2<int> screenOffset = currentScreen->getCameraOffset();
		cameraOffset.x += screenOffset.x;
		cameraOffset.y += screenOffset.y;
	}

	RenderWorld(cameraOffset);

	for (Sprite* currentSprite : m_spriteRenderQueue)
	{
		RenderSprite(currentSprite, cameraOffset);
	}

	for (IScreen* currentScreen : m_screenRenderQueue)
	{
		RenderScreen(currentScreen);
	}

	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwBufferSize = { SCREEN_WIDTH,SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	WriteConsoleOutputA(hOutput, (CHAR_INFO*)m_buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);
}

void Renderer::RenderWorld(const Utility::Vector2<int>& cameraOffset)
{
	for (int x = 0; x < SCREEN_WIDTH; ++x)
	{
		for (int y = 0; y < SCREEN_HEIGHT; ++y)
		{
			Utility::Vector2<int> worldPosition(x - cameraOffset.x, y - cameraOffset.y);

			if (m_world != nullptr && CanAccessMap(*m_world, worldPosition))
			{
				char worldChar = (char)GetWorldMapValue(*m_world, worldPosition);

				m_buffer[y][x].Char.AsciiChar = worldChar;
				m_buffer[y][x].Attributes = WORLD_COLOR;
			}
			else
			{
				m_buffer[y][x].Char.AsciiChar = ' ';
				m_buffer[y][x].Attributes = 0;
			}
		}
	}
}

void Renderer::RenderSprite(Sprite* sprite, const Utility::Vector2<int>& cameraOffset)
{
	if (sprite == nullptr)
	{
		return;
	}

	const SpriteUtils::SpriteDrawing& drawing =
		SpriteUtils::GetSpriteDrawing(sprite->GetType());
	const Utility::Vector2<int> cell = Collision::ToCell(sprite->GetPosition());

	int spriteX = cell.x + cameraOffset.x;
	int spriteY = cell.y + cameraOffset.y;

	for (int y = 0; y < (int)drawing.lines.size(); y++)
	{
		for (int x = 0; x < (int)drawing.lines[y].size(); x++)
		{
			char spriteChar = drawing.lines[y][x];
			int screenX = spriteX + x;
			int screenY = spriteY + y;

			if (spriteChar == ' '
				|| screenX < 0 || screenX >= SCREEN_WIDTH
				|| screenY < 0 || screenY >= PlayArea::HEIGHT)
			{
				continue;
			}

			m_buffer[screenY][screenX].Char.AsciiChar = spriteChar;
			m_buffer[screenY][screenX].Attributes = drawing.color;
		}
	}
}

void Renderer::RenderScreen(const IScreen* screen)
{
	if (screen == nullptr)
	{
		return;
	}

	std::vector<ScreenText> texts = screen->getTexts();
	if (texts.empty())
	{
		return;
	}

	if (screen->DrawsPanel())
	{
		int left = SCREEN_WIDTH;
		int top = SCREEN_HEIGHT;
		int right = 0;
		int bottom = 0;

		for (const ScreenText& text : texts)
		{
			left = (std::min)(left, text.position.x);
			top = (std::min)(top, text.position.y);
			right = (std::max)(right, text.position.x + (int)text.text.size() - 1);
			bottom = (std::max)(bottom, text.position.y);
		}

		left -= PANEL_PADDING_X;
		right += PANEL_PADDING_X;
		top -= PANEL_PADDING_Y;
		bottom += PANEL_PADDING_Y;

		for (int y = top; y <= bottom; ++y)
		{
			for (int x = left; x <= right; ++x)
			{
				if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
				{
					continue;
				}

				bool isVerticalEdge = (x == left || x == right);
				bool isHorizontalEdge = (y == top || y == bottom);

				char panelChar = ' ';
				if (isVerticalEdge && isHorizontalEdge)
				{
					panelChar = '+';
				}
				else if (isHorizontalEdge)
				{
					panelChar = '-';
				}
				else if (isVerticalEdge)
				{
					panelChar = '|';
				}

				m_buffer[y][x].Char.AsciiChar = panelChar;
				m_buffer[y][x].Attributes = PANEL_COLOR;
			}
		}
	}

	for (const ScreenText& text : texts)
	{
		WriteText(text.position.x, text.position.y, text.text, text.color);
	}
}

void Renderer::WriteText(int x, int y, std::string_view text, WORD color)
{
	if (y < 0 || y >= SCREEN_HEIGHT)
	{
		return;
	}

	for (int i = 0; i < (int)text.size(); ++i)
	{
		int screenX = x + i;
		if (screenX < 0 || screenX >= SCREEN_WIDTH)
		{
			continue;
		}

		m_buffer[y][screenX].Char.AsciiChar = text[i];
		m_buffer[y][screenX].Attributes = color;
	}
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
