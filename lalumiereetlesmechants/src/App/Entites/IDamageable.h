#pragma once

class IDamageable {
public:
	virtual ~IDamageable() = default;

	virtual void TakeDamage(const int damages) = 0; //TODO: GO damage INFO POUR FLEX DamgageInfo&
	virtual int GetHealth() const = 0;
	virtual int GetMaxHealth() const = 0;
	virtual bool IsDead() const = 0;

};
