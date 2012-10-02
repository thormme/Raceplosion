#ifndef AIPLAYER_H
#define AIPLAYER_H
#include <zenilib.h>
#include "Player.h"
#include "RaceCar.h"
#include "Tile.h"
#include "NavigationMap.h"

class RaceCar;

class AIPlayer : public Player {
	
private:
	double m_lookAheadTime;
	int m_directionSmoothing;
	std::vector<NavigationMap> m_navigationMaps;

protected:

public:
	AIPlayer();

	virtual const StateModifications driveRaceCar(RaceCar &raceCar, const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);

	void setNavigationMaps(const std::vector<NavigationMap> &navigationMaps);
};
#endif