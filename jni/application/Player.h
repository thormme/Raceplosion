#ifndef PLAYER_H
#define PLAYER_H
#include <zenilib.h>
#include "RaceCar.h"
#include "Tile.h"
#include "Body.h"
#include "Input.h"

class RaceCar;

class Player {
	
private:
	// Player attributes
	InputAction m_forward;
	InputAction m_backward;
	InputAction m_left;
	InputAction m_right;

	RaceCar* m_raceCar;
	// Attributes to pass to car

protected:


public:
	Player();

	RaceCar * getNewCar(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f), const double rotation = 0.0f);

	RaceCar * getLastCar();

	virtual const StateModifications driveRaceCar(RaceCar &raceCar, const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);
};
#endif