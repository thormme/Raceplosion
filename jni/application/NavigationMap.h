#ifndef NAVIGATIONMAP_H
#define NAVIGATIONMAP_H
#include <zenilib.h>

class NavigationMap {
	
private:
	std::vector<std::vector<float>> m_rewards;
	std::vector<std::vector<float>> m_values;

	Zeni::Vector2f m_stateSize;

public:
	NavigationMap();
	NavigationMap(const std::vector<std::vector<float>> &rewards, const Zeni::Vector2f &stateSize);

	void iterateValues();

	float getSuggestedDirectionAtPosition(Zeni::Point2f position) const;
};
#endif