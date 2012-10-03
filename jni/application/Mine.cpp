#include <zenilib.h>
#include "Mine.h"
#include "Explosion.h"
#include "Utils.h"
#include "RaceCar.h"

Mine::Mine(const Actor * owner,
		const Zeni::Point2f &position,
		const double rotation) 
		: m_owner(owner), m_timer(0), Actor(position, Zeni::Vector2f(20.0f, 20.0f), rotation, "weapon_mine", Zeni::Vector2f(0.0f, 0.0f), Zeni::Vector2f(0.0f, 0.0f), 1.0) {
	detectCollisionsWithBodies();
}

const StateModifications Mine::run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	bool explode = false;
	if (bodyCollisions.size()) { // Wait some time before arming
		for (std::vector<Body*>::const_iterator it = bodyCollisions.begin(); it != bodyCollisions.end(); it++) {
			RaceCar * car = dynamic_cast<RaceCar*>(*it);
			if (car != nullptr && (car != m_owner ||  m_timer > .6)) {
				explode = true;
				break;
			}
		}
	}
	if (explode) {
		int numSparks = 9;
		for (int i=0; i < numSparks; i++) {
			double rotation = (double)(i)/(double)(numSparks)*Utils::PI*2.0;
			Explosion* explosion = new Explosion(getPosition() + Utils::getVectorFromAngle(rotation)*(getSize()/2.0f).magnitude(), rotation, Utils::getVectorFromAngle(rotation)*100);
			stateModifications.bodyAdditions.push_back(explosion);
		}
		stateModifications.bodyRemovals.push_back(this);
	}
	return stateModifications;
}

void Mine::stepPhysics(const double timeStep) {
	m_timer += timeStep;
	Actor::stepPhysics(timeStep);
}