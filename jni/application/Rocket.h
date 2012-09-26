#ifndef ROCKET_H
#define ROCKET_H
#include <zenilib.h>
#include "Actor.h"

class Rocket : public Actor {
	
private:
	Zeni::Chronometer<Zeni::Time> m_explosionTimer;

protected:
	virtual const StateModifications run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);

public:
	Rocket(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const double rotation = 0.0f,
		const Zeni::Vector2f &velocity = Zeni::Vector2f(0.0f, 0.0f));

	//virtual void stepPhysics(const double timeStep);
	//virtual void handleCollisions(const double timeStep, std::vector<Tile*>, std::vector<Body*>); ///< Run physics necessary to deal with collisions.

	//virtual ~Rocket();
};
#endif