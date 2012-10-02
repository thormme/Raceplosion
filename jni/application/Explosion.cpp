#include <zenilib.h>
#include "Explosion.h"

const StateModifications Explosion::run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	bool remove = false;
	for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
		if ((*it)->isSolid()) {
			remove = true;
			Tile newTile;
			if ((*it)->isFloating()) {
				newTile = Tile((*it)->getPosition(), (*it)->getSize(), Zeni::String("black"));
			} else {
				newTile = Tile((*it)->getPosition(), (*it)->getSize(), Zeni::String("dirt"));
			}
			stateModifications.tileChanges.push_back(newTile);
		}
	}
	if (m_explosionTimer > .6) {
		remove = true;
	}
	if (remove) {
		stateModifications.bodyRemovals.push_back(this);
	}
	return stateModifications;
}

Explosion::Explosion(const Zeni::Point2f &position,
	const double rotation,
	const Zeni::Vector2f &velocity) 
		: Actor(position, Zeni::Vector2f(16.0f, 16.0f), rotation, "explosion", velocity, Zeni::Vector2f(0.0f, 0.0f), 1.0) {
	setForce(-getRotationVector() * 200);
	detectCollisionsWithBodies();
	detectCollisionsWithTiles();
	m_explosionTimer = 0;
}

void Explosion::stepPhysics(const double timeStep) {
	m_explosionTimer += timeStep;
	Actor::stepPhysics(timeStep);
}