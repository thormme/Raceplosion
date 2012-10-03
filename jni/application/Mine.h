#ifndef MINE_H
#define MINE_H
#include <zenilib.h>
#include "Actor.h"

class Mine : public Actor {
	
private:
	double m_timer;
	const Actor * m_owner;

protected:
	virtual const StateModifications run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);

public:
	Mine(const Actor * owner,
		const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const double rotation = 0.0f);

	virtual void stepPhysics(const double timeStep);
	//virtual void handleCollisions(const double timeStep, std::vector<Tile*>, std::vector<Body*>); ///< Run physics necessary to deal with collisions.

	//virtual ~Rocket();
};
#endif