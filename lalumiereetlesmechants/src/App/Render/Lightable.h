#pragma once

class ILightable
{
private:
	float m_radius{};
public: 
	inline float getRadius() {
		return m_radius;
	}
};

