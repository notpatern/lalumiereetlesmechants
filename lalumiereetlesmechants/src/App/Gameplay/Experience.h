#pragma once
#include <array>

class Experience
{
public:
	static constexpr std::array XP_FOR_NEXT_LEVEL{ 3, 7, 9, 11, 13, 16, 19 };
	static constexpr int MAX_LEVEL = static_cast<int>(XP_FOR_NEXT_LEVEL.size()) + 1;

	void AddXp(int amount);

	int getLevel() const { return m_level; }
	bool IsMaxLevel() const { return m_level == MAX_LEVEL; }
	int getXpInLevel() const { return m_xpInLevel; }
	int getXpForNextLevel() const;

private:
	int m_level = 1;
	int m_xpInLevel = 0;
};
