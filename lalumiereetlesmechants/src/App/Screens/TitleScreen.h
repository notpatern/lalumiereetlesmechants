#pragma once
#include "IScreen.h"
#include "../Manager/InputManager.h"
#include "../Render/Renderer.h"
#include <string_view>
#include <windows.h>

class TitleScreen : public IScreen
{
public:
	void Update(float deltaTime, const InputManager& input) override;
	void Draw(Renderer& renderer) const override;
	void DrawDebug(HANDLE console) const override;
	ScreenRequest getRequest() const override { return m_request; }

private:
	static constexpr std::string_view TITLE_TEXT = "La lumiere et les mechants";
	static constexpr std::string_view START_TEXT = "Espace pour jouer";

	ScreenRequest m_request = ScreenRequest::None;
};
