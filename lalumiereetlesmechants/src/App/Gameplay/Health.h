#pragma once

class Health
{
public:
	explicit Health(int inMaxHealth);

	bool TakeDamage(int inAmount);
	void Heal(int inAmount);

	bool IsDead() const;
	int getCurrent() const;
	int getMax() const;

private:
	int m_max;
	int m_current;
};
