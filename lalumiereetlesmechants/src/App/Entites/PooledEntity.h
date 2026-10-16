#pragma once
#include "Entities.h"

class PooledEntity : public Entity
{
public:
	bool IsActive() const { return m_isActive; }

protected:
	void Activate();
	void Deactivate();

private:
	bool m_isActive = false;
};
