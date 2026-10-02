#pragma once
#include "../../Utilities/DamageInfos.h"

class IDamageable {
public:
	virtual ~IDamageable() = default;

	virtual void TakeDamage(const DamageInfos damagesType) = 0;
	virtual int GetHealth() const = 0;
	virtual int GetMaxHealth() const = 0;
	virtual bool IsDead() const = 0;

};
