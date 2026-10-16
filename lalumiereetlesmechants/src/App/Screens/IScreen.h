#pragma once
#include "ScreenText.h"
#include "../Manager/InputManager.h"
#include <vector>
#include <windows.h>
#include "../../Utilities/Vector2.h"

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
	IScreen();
	virtual ~IScreen();

	// Inscrit dans le Renderer par son adresse, donc pas de copie
	IScreen(const IScreen&) = delete;
	IScreen& operator=(const IScreen&) = delete;

	virtual void Update(float deltaTime, const InputManager& input) = 0;
	virtual std::vector<ScreenText> getTexts() const = 0;
	virtual void DrawDebug(HANDLE console) const = 0;
	virtual ScreenRequest getRequest() const = 0;

	virtual bool DrawsPanel() const { return true; }
	virtual Utility::Vector2<int> getCameraOffset() const { return { 0, 0 }; }
};
