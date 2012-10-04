#include <zenilib.h>
#include "Sensor.h"
#include "Body.h"

Sensor::Sensor(Player * owner, const Zeni::Vector2f &size) 
	: m_owner(owner), 
	  Body(owner->getLastCar()->getCenter() - size/2.0f, size) {
	
}

void Sensor::handleCollisions(const double timeStep, std::vector<Tile*> tiles, std::vector<Body*> bodies) {
	m_owner->senseSurroundings(tiles, bodies);
	setPosition(m_owner->getLastCar()->getCenter() - getSize()/2.0f);
}

void Sensor::render() {}