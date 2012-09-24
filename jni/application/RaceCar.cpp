#include <zenilib.h>
#include <cmath>
#include <sstream>
#include "Utils.h"
#include "RaceCar.h"
#include "Input.h"
#include "PlayState.h"
#include "Rocket.h"

namespace {
	const double FRICTION_COEFFICIENT = 1000.0;
}

const Zeni::Vector2f RaceCar::getDirectionalVelocity(const double &direction) {
	Zeni::Vector2f directionVector(cos(direction), sin(direction));
	return (getVelocity() * directionVector) / directionVector.magnitude2() * directionVector;
}

const Zeni::Vector2f RaceCar::getHorizontalFrictionForce() {
	Zeni::Vector2f perpendicularVelocity = getVelocity() - getDirectionalVelocity(getRotation());
	return -perpendicularVelocity.normalized()*FRICTION_COEFFICIENT*m_friction*getMass();
}

// Causes the actor to take an action.
const StateModifications RaceCar::run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	setForce(Zeni::Vector2f(0.0f, 0.0f));
	m_wheelRotation = 0;

	if (Input::isKeyDown(SDLK_UP)) {
		setForce(getRotationVector() * 200);
	}

	if (Input::isKeyDown(SDLK_DOWN)) {
		setForce(getRotationVector() * -200);
	}

	if (Input::isKeyDown(SDLK_LEFT)) {
		m_wheelRotation = Utils::PI/6;
	}

	if (Input::isKeyDown(SDLK_RIGHT)) {
		m_wheelRotation = -Utils::PI/6;
	}

	if (Input::isKeyPressed(SDLK_w)) {
		Rocket * rocket = new Rocket(getCenter(), getRotation(), getVelocity());
		rocket->setPosition(rocket->getPosition() - rocket->getSize()/2.0f);
		stateModifications.bodyAdditions.push_back(rocket);
	}

	if (Input::isKeyDown(SDLK_q)) {
		for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
			Tile newTile = Tile((*it)->getPosition(), (*it)->getSize(), Zeni::String("grass"));
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
		setRotationRate(1.0/(m_wheelSeparation/cos(m_wheelRotation + Utils::PI/2.0))*directionVelocity);
	} else {
		setRotationRate(0);
	}
	if (getDirectionalVelocity(getRotation()+Utils::PI/2).magnitude() > 1) setForce(getForce()  + getHorizontalFrictionForce());
	/*std::ostringstream str;
	str << getDirectionalVelocity(getRotation()+PI/2).magnitude() << "\n";
	OutputDebugString( str.str().c_str());*/
	Actor::stepPhysics(timeStep);
}

void RaceCar::handleCollisions(const double timeStep, std::vector<Tile*> tiles, std::vector<Body*> bodies) {
	m_friction = 0;
	for (int i=0; i < tiles.size(); i++) {
		if (tiles[i]->isSolid()) {
			// Hit a wall
			std::list<Zeni::Point2f> points;
			// TODO: Store bounding points in tile so that these assumptions don't have to be made.
			points.push_back(tiles[i]->getPosition());
			points.push_back(tiles[i]->getPosition() + tiles[i]->getSize());
			points.push_back(Zeni::Point2f(tiles[i]->getPosition().x + tiles[i]->getSize().i, tiles[i]->getPosition().y));
			points.push_back(Zeni::Point2f(tiles[i]->getPosition().x, tiles[i]->getPosition().y +tiles[i]->getSize().j));
			Collision collision = getCollision(points, true);
			Zeni::Vector2f velocityTowardTile = getDirectionalVelocity(Utils::getAngleFromVector((tiles[i]->getPosition() + tiles[i]->getSize()/2.0f) - getCenter()));
			setVelocity(getVelocity() - velocityTowardTile);
			double heightDifference = getSize().j/2.0 - collision.heightDistanceVector.magnitude();
			double widthDifference = getSize().i/2.0 - collision.widthDistanceVector.magnitude();
			Zeni::Vector2f positionModifier = heightDifference < widthDifference ? collision.heightDistanceVector.normalized()*heightDifference : collision.widthDistanceVector.normalized()*widthDifference;
			// TODO: This should NOT be necessary!
			if (collision.isColliding) setPosition(getPosition() + positionModifier);
		}
		if (tiles[i]->getImage().compare("grass") == 0) {
			m_friction += .5;
		} else {
			m_friction += 1.0;
		}
	}
	if (tiles.size()) m_friction /= tiles.size();
}

RaceCar::RaceCar(const Zeni::Point2f &position,
		const double rotation,
		const Zeni::String image)
		: Actor(position, Zeni::Vector2f(64.0f, 32.0f), rotation, image, Zeni::Vector2f(0.0f, 0.0f), Zeni::Vector2f(0.0f, 0.0f), 1.0) {
	m_wheelSeparation = 64.0;
	detectCollisionsWithBodies();
	detectCollisionsWithTiles();
	m_startPosition = getPosition();
}

// If you might delete base class pointers, you need a virtual destructor.
RaceCar::~RaceCar() {}