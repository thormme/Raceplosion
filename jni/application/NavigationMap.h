#ifndef NAVIGATIONMAP_H
#define NAVIGATIONMAP_H
#include <zenilib.h>
#include "Tile.h"

class NavigationMap {
	
private:
	std::vector<std::vector<float>> m_rewards;
	std::vector<std::vector<float>> m_values;

	Zeni::Vector2f m_stateSize;

public:
	NavigationMap();
	NavigationMap(const std::vector<std::vector<Tile*>> &tiles, const Zeni::Vector2f &stateSize, const std::vector<Zeni::Point2f> &goals);

	void iterateValues();

	float getSuggestedDirectionAtPosition(Zeni::Point2f position, const int smoothing = 2) const;
	float getValueAtPosition(Zeni::Point2f position) const;
};
#endif