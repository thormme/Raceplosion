#include <zenilib.h>
#include "Rocket.h"


Rocket::Rocket(const Zeni::Point2f &position,
		const double rotation,
		const Zeni::Vector2f &velocity) 
		: Actor(position, Zeni::Vector2f(32.0f, 8.0f), rotation, "weapon_rocket", velocity, Zeni::Vector2f(0.0f, 0.0f), 1.0) {
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
	if (explode) {
		for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
			Tile newTile = Tile((*it)->getPosition(), (*it)->getSize(), Zeni::String("grass"));
			stateModifications.tileChanges.push_back(newTile);
		}
		stateModifications.bodyRemovals.push_back(this);
	}
	return stateModifications;
}

//void Rocket::stepPhysics(const double timeStep){}
//void Rocket::handleCollisions(const double timeStep, std::vector<Tile*>, std::vector<Body*>){}

//Rocket::~Rocket(){}