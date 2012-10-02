#include <zenilib.h>
#
#include "NavigationMap.h"
#include "Utils.h"

NavigationMap::NavigationMap() {

}

float getRewardFromTile(const Tile &tile) {
	float reward = 0;
	if (tile.isPit()) {
		reward = -100;
	}
	if (tile.isSolid()) {
		reward = -100;
	}
	return reward;
}

NavigationMap::NavigationMap(const std::vector<std::vector<Tile*>> &tiles, const Zeni::Vector2f &stateSize, const std::vector<Zeni::Point2f> &goals) {
	for (int i=0; i < tiles.size(); i++) {
		std::vector<float> rewardsRow;
		for (int j=0; j < tiles[i].size(); j++) {
			rewardsRow.push_back(getRewardFromTile(*tiles[i][j]));
		}
		m_rewards.push_back(rewardsRow);
	}
	for (int i=0; i < goals.size(); i++) {
		if (goals[i].x >= 0 && goals[i].x/stateSize.i < m_rewards.size() && goals[i].y >= 0 && goals[i].y/stateSize.j < m_rewards[goals[i].x/stateSize.i].size()) {
			m_rewards[goals[i].x/stateSize.i][goals[i].y/stateSize.j] = 10;
		}
	}
	m_stateSize = stateSize;

	m_values = m_rewards;
}

void NavigationMap::iterateValues() {
	std::vector<std::vector<float>> startingValues(m_values.begin(), m_values.end());
	double discount = .999;
	for (int i=1; i < m_values.size() - 1; i++) {
		for (int j=1; j < m_values[i].size() - 1; j++) {
			float max = std::max(startingValues[i][j-1] * discount + m_rewards[i][j-1], 
						std::max(startingValues[i][j+1] * discount + m_rewards[i][j+1], 
						std::max(startingValues[i-1][j] * discount + m_rewards[i-1][j], 
								 startingValues[i+1][j] * discount + m_rewards[i+1][j])));
			m_values[i][j] = 0;
			
			m_values[i][j] += (startingValues[i][j-1] * discount + m_rewards[i][j-1]) * .1;
			m_values[i][j] += (startingValues[i][j+1] * discount + m_rewards[i][j+1]) * .1;
			m_values[i][j] += (startingValues[i-1][j] * discount + m_rewards[i-1][j]) * .1;
			m_values[i][j] += (startingValues[i+1][j] * discount + m_rewards[i+1][j]) * .1;

			m_values[i][j] -= max * .1; // Give chosen direction 70% likelihood
			m_values[i][j] += max * .7;
		}
	}
}

float NavigationMap::getSuggestedDirectionAtPosition(Zeni::Point2f position, const int smoothing) const {
	Zeni::Vector2f pos = Zeni::Vector2f(position).divide_by(m_stateSize);
	float maxValue = -1000000.0;
	float direction = 0;
	for (int x = -smoothing; x <= smoothing; x++) {
		for (int y = -smoothing; y <= smoothing; y++) {
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

float NavigationMap::getValueAtPosition(Zeni::Point2f position) const {
	Zeni::Vector2f pos1 = Zeni::Vector2f(position).divide_by(m_stateSize);
	Zeni::Vector2f pos(pos1.i, pos1.j);
	if ((int)pos.i >= 0 && (int)pos.i < m_values.size()) {
		if ((int)pos.j >= 0) {
			if ((int)pos.j < m_values[(int)pos.i].size()) {
				return m_values[(int)pos.i][(int)pos.j];
			}
		}
	}
	return 0;
}