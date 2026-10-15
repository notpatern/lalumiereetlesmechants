#pragma once
#include "IScreen.h"
#include "Menu.h"
#include "../Manager/InputManager.h"
#include <array>
#include <string_view>
#include <windows.h>
#include <vector>

class CreditsScreen : public IScreen
{
public:
	void Update(float deltaTime, const InputManager& input) override;
	std::vector<ScreenText> getTexts() const override;
	void DrawDebug(HANDLE console) const override;
	ScreenRequest getRequest() const override { return m_request; }

private:
	static constexpr std::array<std::string_view, 7> LINES =
	{
		"La lumiere et les mechants",
		"",
		"Paul",
		"Sasha",
		"",
		"ENJMIN / Cnam, octobre 2026",
		"Merci a Axel Buendia",
	};

	ScreenRequest m_request = ScreenRequest::None;
	Menu m_menu{ "Retour" };
};
