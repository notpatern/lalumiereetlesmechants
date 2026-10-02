#pragma once

class Health
{
public:
	explicit Health(int maxHealth);

	bool TakeDamage(int amount);
	void Reset();

	bool IsDead() const;
	int getCurrent() const;
	int getMax() const;

private:
	int m_max;
	int m_current;
};
