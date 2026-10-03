#pragma once
#include <concepts>
#include "Collision.h"
#include "IDamageable.h"

template <typename T>
concept Hittable = std::derived_from<T, IDamageable> && requires(const T& target)
{
	{ target.getIsActive() } -> std::same_as<bool>;
	{ target.getHitboxRect() } -> std::same_as<Collision::CellRect>;
};
