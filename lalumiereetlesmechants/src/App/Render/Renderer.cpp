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



	WriteConsoleOutput(hOutput, (CHAR_INFO*)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);


	for (Sprite* currentSprite : m_renderQueue) {
		currentSprite->Render();
	}
}

void Renderer::AddToRenderQueue(Sprite* sprite)
{
	m_renderQueue.push_back(sprite);
}
