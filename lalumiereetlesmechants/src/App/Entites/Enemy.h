#pragma once
#include "Entities.h"
#include "IDamageable.h"
#include "../Render/Lightable.h"

class Enemy : public Entity, ILightable, public IDamageable
{
public:
	Enemy();
	Enemy(const Utility::Vector2<int>& position, Sprite* sprite);
	~Enemy();

	void Update() override;

	//IDamageable
	void TakeDamage(const int damages) override;
	int GetHealth() const override {return m_health;}
	int GetMaxHealth() const override {return m_maxHealth;}
	bool IsDead() const override {return m_isDead;}

private:
	int m_maxHealth{1};
	int m_health{m_maxHealth};
	bool m_isDead{false};

	void Die();

};

