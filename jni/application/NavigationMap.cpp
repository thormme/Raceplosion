#include <zenilib.h>
#include "NavigationMap.h"
#include "Utils.h"

NavigationMap::NavigationMap() {

}

NavigationMap::NavigationMap(const std::vector<std::vector<float>> &rewards, const Zeni::Vector2f &stateSize) {
	m_rewards = rewards;
	m_stateSize = stateSize;

	m_values = rewards;
}

void NavigationMap::iterateValues() {
	std::vector<std::vector<float>> startingValues(m_values.begin(), m_values.end());
	for (int i=1; i < m_values.size() - 1; i++) {
		for (int j=1; j < m_values.size() - 1; j++) {
			float max = std::max(startingValues[i][j-1] * .9 + m_rewards[i][j-1], 
						std::max(startingValues[i][j+1] * .9 + m_rewards[i][j+1], 
						std::max(startingValues[i-1][j] * .9 + m_rewards[i-1][j], 
								 startingValues[i+1][j] * .9 + m_rewards[i+1][j])));
			m_values[i][j] = 0;
			
			m_values[i][j] += (startingValues[i][j-1] * .9 + m_rewards[i][j-1]) * .1;
			m_values[i][j] += (startingValues[i][j+1] * .9 + m_rewards[i][j+1]) * .1;
			m_values[i][j] += (startingValues[i-1][j] * .9 + m_rewards[i-1][j]) * .1;
			m_values[i][j] += (startingValues[i+1][j] * .9 + m_rewards[i+1][j]) * .1;

			m_values[i][j] -= max * .1; // Give chosen direction 70% likelihood
			m_values[i][j] += max * .7;
		}
	}
}

float NavigationMap::getSuggestedDirectionAtPosition(Zeni::Point2f position) const {
	Zeni::Vector2f pos = Zeni::Vector2f(position).divide_by(m_stateSize);
	float maxValue = -10000.0;
	float direction = 0;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x != 0 || y != 0) {
				if (x + (int)pos.i >= 0) {
					if (x + (int)pos.i < m_values.size()) {
						if (y + (int)pos.j >= 0) {
							if (y + (int)pos.j < m_values[x + (int)pos.i].size()) {
								if (m_values[x + (int)pos.i][y + (int)pos.j] > maxValue) {
									maxValue = m_values[x + (int)pos.i][y + (int)pos.j];
									direction = Utils::getAngleFromVector(Zeni::Vector2f(x, y));
								}
							}
						}
					}
				}
			}
		}
	}
	return direction;
}