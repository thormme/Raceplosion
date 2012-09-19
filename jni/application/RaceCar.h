#ifndef RACECAR_H
#define RACECAR_H
#include <zenilib.h>
#include "Actor.h"

class RaceCar : public Actor {
	
private:
	double m_wheelRotation;
	double m_wheelSeparation;

	const Zeni::Vector2f getHorizontalFrictionForce();
	const Zeni::Vector2f getDirectionalVelocity(const double &direction);

protected:
	virtual const StateModifications run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);

public:
	RaceCar(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const double rotation = 0.0f,
		const Zeni::String image = "race_car");

	virtual void stepPhysics(const double timeStep);
	virtual void handleCollisions(const double timeStep, std::vector<Tile*>, std::vector<Body*>); ///< Run physics necessary to deal with collisions.

	virtual ~RaceCar();
};
#endif