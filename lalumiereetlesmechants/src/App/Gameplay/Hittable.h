#pragma once
#include "Collision.h"
#include "IDamageable.h"
#include <concepts>

template <typename T>
concept Hittable = std::derived_from<T, IDamageable> && requires(const T& inTarget)
{
	{ inTarget.IsActive() } -> std::same_as<bool>;
	{ inTarget.getHitboxRect() } -> std::same_as<Collision::CellRect>;
};
