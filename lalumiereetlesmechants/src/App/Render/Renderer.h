#pragma once

#include "../World/World.h"
#include "Sprite.h"
#include "../Screens/IScreen.h"
#include "../Render/SpriteDrawing.h"
#include "../Gameplay/PlayArea.h"
#include <string_view>
#include <vector>
#include <windows.h>

class Renderer // il render le monde, les sprites et les ecrans. Ses render queue se remplissent et se vident automatiquement avec les instanciation et destructions des Sprites et Screens
{
public:
	static constexpr short SCREEN_WIDTH = 80;
	static constexpr short SCREEN_HEIGHT = PlayArea::HEIGHT + 2;

private:
	static constexpr int PANEL_PADDING_X = 3;
	static constexpr int PANEL_PADDING_Y = 1;
	static constexpr WORD PANEL_COLOR = 0x0F;
	static constexpr WORD WORLD_COLOR = 0x0F;

	World* m_world = nullptr;
	std::vector<Sprite*> m_spriteRenderQueue{};
	std::vector<IScreen*> m_screenRenderQueue{};

	CHAR_INFO m_buffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {};

	void RenderWorld(const Utility::Vector2<int>& cameraOffset);
	void RenderScreen(const IScreen* screen);
	void WriteText(int x, int y, std::string_view text, WORD color);

public:
	Renderer();
	Renderer(World* world);
	~Renderer();

	static Renderer& getInstance()
	{
		static Renderer instance;
		return instance;
	}

	void SetWorld(World* world) { m_world = world; }
	void RenderSprite(Sprite* sprite, const Utility::Vector2<int>& cameraOffset);

	void Render();
	void RenderSprite(Sprite* sprite);
	void AddToRenderQueue(Sprite* sprite);
	void AddToRenderQueue(IScreen* screen);
	void RemoveFromRenderQueue(Sprite* sprite);
	void RemoveFromRenderQueue(IScreen* screen);
};
