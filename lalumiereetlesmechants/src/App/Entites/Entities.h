#pragma once

#include "../Render/Sprite.h"
#include "../../Utilities/Vector2.h"

namespace Entity {
	class Entities
	{
	protected:
		Utilities::Vector2<int>* m_position;
		Sprite* m_sprite;

	public:
		Entities();
		Entities(Utilities::Vector2<int>* position, Sprite* sprite);
		~Entities();

		inline Sprite* getSprite() {
			return m_sprite;
		}

		inline void setSprite(Sprite* sprite) {
			m_sprite = sprite;
		}

		inline Utilities::Vector2<int>* getPosition() {
			return m_position;
		}

		void setPosition(Utilities::Vector2<int>* newPos) {
			m_position = newPos;
		}

		void setPosition(Utilities::Vector2<float>* newPos) {
			m_position->setX((int)newPos->getX());
			m_position->setY((int)newPos->getY());
		}

		void setPosition(int x, int y) {
			m_position->setX(x);
			m_position->setY(y);
		}

		virtual void Update() = 0;
	};
}
