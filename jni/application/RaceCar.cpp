#include <zenilib.h>
#include <cmath>
#include <sstream>
#include "RaceCar.h"
#include "Input.h"
#include "PlayState.h"

namespace {
	const double PI = 3.141592653589793238462;
}

const Zeni::Vector2f RaceCar::getDirectionalVelocity(const double &direction) {
	Zeni::Vector2f directionVector(cos(direction), sin(direction));
	return (getVelocity() * directionVector) / directionVector.magnitude() * directionVector;
}

const Zeni::Vector2f RaceCar::getHorizontalFrictionForce() {
	Zeni::Vector2f perpendicularVelocity = getVelocity() - getDirectionalVelocity(getRotation());
	return -perpendicularVelocity.normalized()*1000/*friction coefficient*/*getMass();
}

// Causes the actor to take an action.
const StateModifications RaceCar::run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	setForce(Zeni::Vector2f(0.0f, 0.0f));
	m_wheelRotation = 0;

	if (Input::isKeyDown(SDLK_UP)) {
		setForce(Zeni::Vector2f(cos(getRotation()), sin(getRotation())) * 200);
	}

	if (Input::isKeyDown(SDLK_DOWN)) {
		setForce(Zeni::Vector2f(cos(getRotation()), sin(getRotation())) * -200);
	}

	if (Input::isKeyDown(SDLK_LEFT)) {
		m_wheelRotation = PI/6;
	}

	if (Input::isKeyDown(SDLK_RIGHT)) {
		m_wheelRotation = -PI/6;
	}

	if (Input::isKeyPressed(SDLK_w)) {
		stateModifications.bodyAdditions.push_back(new Body(getPosition()));
	}

	if (Input::isKeyDown(SDLK_q)) {
		for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
			Tile newTile = Tile((*it)->getPosition(), Zeni::String("grass"));
			stateModifications.tileChanges.push_back(newTile);
		}
	}
	return stateModifications;
}

void RaceCar::stepPhysics(const double timeStep) {
	if (m_wheelRotation != 0) {
		Zeni::Vector2f velocity = getDirectionalVelocity(getRotation());
		Zeni::Vector2f directionVector = Zeni::Vector2f(cos(getRotation()), sin(getRotation()));
		double directionVelocity;
		if ((velocity - directionVector).magnitude() < (velocity - directionVector*-1).magnitude()) {
			directionVelocity = getDirectionalVelocity(getRotation()).magnitude();
		} else {
			directionVelocity = -getDirectionalVelocity(getRotation()).magnitude();
		}
		setRotationRate(1.0/(m_wheelSeparation/cos(m_wheelRotation + PI/2.0))*directionVelocity);
	} else {
		setRotationRate(0);
	}
	if (getDirectionalVelocity(getRotation()+PI/2).magnitude() > 1) setForce(getForce()  + getHorizontalFrictionForce());
	/*std::ostringstream str;
	str << getDirectionalVelocity(getRotation()+PI/2).magnitude() << "\n";
	OutputDebugString( str.str().c_str());*/
	Actor::stepPhysics(timeStep);
}

void RaceCar::handleCollisions(const double timeStep, std::vector<Tile*> tiles, std::vector<Body*> bodies) {
	for (int i=0; i < tiles.size(); i++) {
		if (tiles[i]->getImage().compare("placeholder") == 0) {
			setVelocity(Zeni::Vector2f(0.0, 0.0));
		}
	}
}

RaceCar::RaceCar(const Zeni::Point2f &position,
		const double rotation,
		const Zeni::String image)
		: Actor(position, Zeni::Vector2f(64.0f, 32.0f), rotation, image, Zeni::Vector2f(0.0f, 0.0f), Zeni::Vector2f(0.0f, 0.0f), 1.0) {
	m_wheelSeparation = 64.0;
	detectCollisionsWithBodies();
	detectCollisionsWithTiles();
}

// If you might delete base class pointers, you need a virtual destructor.
RaceCar::~RaceCar() {}