#pragma once
#include "../../Utilities/Vector2.h"

enum class EnemyRemovalReason
{
	Killed,
	Escaped
};

struct EnemyRemovedEvent
{
	EnemyRemovalReason reason = EnemyRemovalReason::Killed;
	Utility::Vector2<float> position;
};

class IEnemyObserver
{
public:
	virtual ~IEnemyObserver() = default;

	virtual void OnEnemyRemoved(const EnemyRemovedEvent& event) = 0;
};
