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
	bool m_fireRocket;
	bool m_layMine;
	double m_rocketTimer;
	double m_mineTimer;
	std::vector<NavigationMap> m_navigationMaps;

	bool m_offensive;

protected:

public:
	AIPlayer(bool offensive = true);

	virtual const StateModifications driveRaceCar(RaceCar &raceCar, const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);

	virtual void senseSurroundings(std::vector<Tile*> tiles, std::vector<Body*> bodies);

	void setNavigationMaps(const std::vector<NavigationMap> &navigationMaps);
};
#endif