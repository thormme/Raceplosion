#include <zenilib.h>
#include "Rocket.h"
#include "Explosion.h"
#include "Utils.h"
#include "RaceCar.h"

Rocket::Rocket(const Zeni::Point2f &position,
		const double rotation,
		const Zeni::Vector2f &velocity) 
		: Actor(position, Zeni::Vector2f(32.0f, 8.0f), rotation, "weapon_rocket", velocity, Zeni::Vector2f(0.0f, 0.0f), 1.0) {
	setForce(getRotationVector() * 500);
	detectCollisionsWithBodies();
	detectCollisionsWithTiles();
	m_explosionTimer.start();
}

const StateModifications Rocket::run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	bool explode = false;
	for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
		if ((*it)->isSolid()) {
			explode = true;
			break;
		}
	}
	if (bodyCollisions.size() && m_explosionTimer.seconds() > .6) {
		for (std::vector<Body*>::const_iterator it = bodyCollisions.begin(); it != bodyCollisions.end(); it++) {
			RaceCar * car = dynamic_cast<RaceCar*>(*it);
			if (car != nullptr) {
				explode = true;
				break;
			}
		}
	}
	if (explode) {
		int numSparks = 9;
		for (int i=0; i < 9; i++) {
			double rotation = (double)(i)/(double)(numSparks)*Utils::PI*2.0;
			Explosion* explosion = new Explosion(getPosition() + Utils::getVectorFromAngle(rotation)*(getSize()/2.0f).magnitude(), rotation, Utils::getVectorFromAngle(rotation)*100);
			stateModifications.bodyAdditions.push_back(explosion);
		}
		stateModifications.bodyRemovals.push_back(this);
	}
	return stateModifications;
}

//void Rocket::stepPhysics(const double timeStep){}
//void Rocket::handleCollisions(const double timeStep, std::vector<Tile*>, std::vector<Body*>){}