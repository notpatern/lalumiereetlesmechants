#pragma once
#include "../../Utilities/Countdown.h"
#include "../../Utilities/Vector2.h"

class Sprite; //TODO : DEMANDER FORWARD DECLARATION PROF

class Missile
//TODO : DEMANDER A SASHA CE QUIL AVAIT PREVU POUR LHERITAGE PCQ JE SAIS PAS COMMENT FAIRE AVEC SES FONCTIONS FZKANFGEZGNB
{
public:
	Missile();

	Sprite* getSprite() const { return m_sprite; }
	Utility::Vector2<float> getPosition() const { return m_position; }

	void Update(float dt);

	bool getIsActive() const { return m_isActive; }

	void Launch(const Utility::Vector2<float>& position, const Utility::Vector2<float>& velocity);
	void Deactivate();

private:
	Sprite* m_sprite = nullptr;

	Utility::Vector2<float> m_position;
	Utility::Vector2<float> m_velocity;

	static constexpr float LIFETIME = 2.f;
	Utility::Countdown m_lifetime;

	bool m_isActive;
};

