#pragma once
#include "IScreen.h"
#include "Menu.h"
#include "ScreenText.h"
#include "../Manager/InputManager.h"
#include <array>
#include <string_view>
#include <vector>
#include <windows.h>

class CreditsScreen : public IScreen
{
public:
	void Update(float inDeltaTime, const InputManager& inInput) override;

	std::vector<ScreenText> getTexts() const override;
	ScreenRequest getRequest() const override { return m_request; }

private:
	static constexpr std::array<std::string_view, 7> LINES =
	{
		"MAUVAIS CARACTERES",
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
