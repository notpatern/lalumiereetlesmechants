#pragma once
#include "../Manager/InputManager.h"
#include "../Render/Renderer.h"
#include <windows.h>

enum class ScreenRequest
{
	None,
	NewGame,
	Title,
	Credits,
	Quit
};

class IScreen
{
public:
	virtual ~IScreen() = default;

	virtual void Update(float deltaTime, const InputManager& input) = 0;
	virtual void Draw(Renderer& renderer) const = 0;
	virtual void DrawDebug(HANDLE console) const = 0;
	virtual ScreenRequest getRequest() const = 0;
};
