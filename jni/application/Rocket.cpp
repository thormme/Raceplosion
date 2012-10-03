#include <zenilib.h>
#include "Rocket.h"
#include "Explosion.h"
#include "Utils.h"
#include "RaceCar.h"

Rocket::Rocket(const Actor * owner,
		const Zeni::Point2f &position,
		const double rotation,
		const Zeni::Vector2f &velocity) 
		: m_owner(owner), Actor(position, Zeni::Vector2f(32.0f, 8.0f), rotation, "weapon_rocket", velocity, Zeni::Vector2f(0.0f, 0.0f), 1.0) {
	setForce(getRotationVector() * 500);
	detectCollisionsWithBodies();
	detectCollisionsWithTiles();
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
	if (bodyCollisions.size()) {
		for (std::vector<Body*>::const_iterator it = bodyCollisions.begin(); it != bodyCollisions.end(); it++) {
			RaceCar * car = dynamic_cast<RaceCar*>(*it);
			if (car != nullptr && car != m_owner) {
				explode = true;
				setPosition(car->getPosition()); // Explode on top of car
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

//void Rocket::stepPhysics(const double timeStep){}
//void Rocket::handleCollisions(const double timeStep, std::vector<Tile*>, std::vector<Body*>){}