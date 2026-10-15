#pragma once
#include "DamageInfos.h"

class IDamageable
{
public:
	virtual ~IDamageable() = default;

	virtual DamageResult TakeDamage(const DamageInfos& damage) = 0;
	virtual bool IsDead() const = 0;
};
