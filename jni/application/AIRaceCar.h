#ifndef AIRACECAR_H
#define AIRACECAR_H
#include <zenilib.h>
#include "RaceCar.h"
#include "Waypoint.h"
#include "NavigationMap.h"

class AIRaceCar : public RaceCar {
	
private:
	std::vector<NavigationMap> m_navigationMaps;

protected:
	virtual const StateModifications run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);

public:
	AIRaceCar(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const double rotation = 0.0f,
		const Zeni::String image = "race_car");

	void setNavigationMaps(const std::vector<NavigationMap> &navigationMaps);
};
#endif