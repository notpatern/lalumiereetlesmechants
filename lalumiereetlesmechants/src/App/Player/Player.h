#pragma once
#include "../../Utilities/Vector2.h"
#include "../Manager/InputManager.h"
#include "../Render/Sprite.h"

class Player
{
public:
	explicit Player(const Utility::Vector2<float>& startPosition);

	void Update(float dt, const InputManager& input);

	Utility::Vector2<float> getPosition() const { return m_position; }
	Sprite* getSprite() const { return m_sprite; }

private:
	Sprite* m_sprite = nullptr;
	Utility::Vector2<float> m_position;
	Utility::Vector2<float> m_speed{ 30.f, 15.f }; // TODO : DEMANDER A SASHA SI C'EST BIEN CA POUR HAUTEUR > LARGEUR

	static Utility::Vector2<int> ReadDirection(const InputManager& input);
	void Move(float dt, const Utility::Vector2<int>& direction);
};
