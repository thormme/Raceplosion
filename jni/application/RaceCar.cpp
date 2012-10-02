#include <zenilib.h>
#include <cmath>
#include <sstream>
#include "Utils.h"
#include "RaceCar.h"
#include "Input.h"
#include "PlayState.h"
#include "Rocket.h"
#include "Player.h"

const Zeni::Vector2f RaceCar::getDirectionalVelocity(const double &direction) {
	Zeni::Vector2f directionVector(cos(direction), sin(direction));
	return (getVelocity() * directionVector) / directionVector.magnitude2() * directionVector;
}

const Zeni::Vector2f RaceCar::getDirectionalFrictionForce(const double &direction, const double &friction) {
	Zeni::Vector2f perpendicularVelocity = getDirectionalVelocity(direction);
	return -perpendicularVelocity.normalized()*friction*m_traction*getMass();
}

const bool RaceCar::isMovingInDirection(const double &direction) {
	Zeni::Vector2f velocity = getDirectionalVelocity(direction);
	Zeni::Vector2f directionVector = Utils::getVectorFromAngle(direction);
	return (velocity - directionVector).magnitude() < (velocity - directionVector*-1.0).magnitude();
}

void RaceCar::setBraking(bool brake) {
	m_braking = brake;
}

void RaceCar::accelerate(double fraction) {
	setForce(getRotationVector() * m_engineForce * fraction);
}

void RaceCar::setWheelRotation(double rotation) {
	m_wheelRotation = rotation;
}

Rocket* RaceCar::fireRocket() {
	Rocket * rocket = new Rocket(getCenter(), getRotation(), getDirectionalVelocity(getRotation()));
	rocket->setPosition(rocket->getPosition() - rocket->getSize()/2.0f);
	return rocket;
}

// Causes the actor to take an action.
const StateModifications RaceCar::run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	if (m_driver != nullptr) {
		stateModifications.combine(m_driver->driveRaceCar(*this, tileCollisions, bodyCollisions));
	}
	return stateModifications;
}

void RaceCar::stepPhysics(const double timeStep) {
	// Turn the car
	if (m_wheelRotation != 0) {
		Zeni::Vector2f velocity = getDirectionalVelocity(getRotation());
		Zeni::Vector2f directionVector = Zeni::Vector2f(cos(getRotation()), sin(getRotation()));
		double directionVelocity;
		if (isMovingInDirection(getRotation())) {
			directionVelocity = getDirectionalVelocity(getRotation()).magnitude();
		} else {
			directionVelocity = -getDirectionalVelocity(getRotation()).magnitude();
		}
		setRotationRate(1.0/(m_wheelSeparation/cos(m_wheelRotation + Utils::PI/2.0))*directionVelocity);
	} else {
		setRotationRate(0);
	}
	
	if (m_braking) {
		// Apply braking friction
		if (getVelocity().magnitude() > 1) {
			setForce(getForce() + getDirectionalFrictionForce(Utils::getAngleFromVector(getVelocity()), m_tireFriction));
		}
	} else {
		// Apply horizontal friction
		if (getDirectionalVelocity(getRotation()+Utils::PI/2).magnitude() > 1) {
			setForce(getForce() + getDirectionalFrictionForce(getRotation() + Utils::PI/2.0f, m_tireFriction));
		}
		// Apply rolling friction
		if (getDirectionalVelocity(getRotation()).magnitude() > 1) {
			setForce(getForce() + getDirectionalFrictionForce(getRotation(), m_rollingFriction));
		}
	}
	Actor::stepPhysics(timeStep);
}

void RaceCar::handleCollisions(const double timeStep, std::vector<Tile*> tiles, std::vector<Body*> bodies) {
	m_traction = 0;
	for (int i=0; i < tiles.size(); i++) {
		if (tiles[i]->isSolid()) {
			// Hit a wall
			Collision collision = getCollision(tiles[i]->getBoundingPoints(), true);
			Collision otherCollision = tiles[i]->getCollision(getBoundingPoints(), true);
			if (otherCollision.minimumSeparation.magnitude2() < collision.minimumSeparation.magnitude2()) {
				collision = otherCollision;
				collision.minimumSeparation = -collision.minimumSeparation;
			}
			Zeni::Vector2f velocityTowardTile = getDirectionalVelocity(Utils::getAngleFromVector((tiles[i]->getPosition() + tiles[i]->getSize()/2.0f) - getCenter()));
			setVelocity(getVelocity() - velocityTowardTile);
			// TODO: This should NOT be necessary!
			if (collision.isColliding) setPosition(getPosition() + collision.minimumSeparation);
		}
		if (tiles[i]->getImage().compare("grass") == 0) {
			m_traction += .5;
		} else {
			m_traction += 1.0;
		}
	}
	if (tiles.size()) m_traction /= tiles.size();

	for (std::vector<Body*>::iterator it = bodies.begin(); it != bodies.end(); it++) {
		Waypoint * waypoint = dynamic_cast<Waypoint*>(*it);
		if (waypoint != nullptr) {
			bool notYetSeen = true;
			for (std::vector<Waypoint*>::iterator waypointIterator = m_passedWaypoints.begin(); waypointIterator != m_passedWaypoints.end(); waypointIterator++) {
				if (*waypointIterator == waypoint) {
					notYetSeen = false;
					break;
				}
			}
			if (notYetSeen) {
				m_passedWaypoints.push_back(waypoint);
			}
		}
	}
}

const int RaceCar::getCompletedLaps() const {
	return m_completedLaps;
}

const std::vector<Waypoint*> RaceCar::getPassedWaypoints() const {
	return m_passedWaypoints;
}

void RaceCar::setLapCompleted() {
	m_passedWaypoints.clear();
	m_completedLaps++;
}

RaceCar::RaceCar(const Zeni::Point2f &position,
		const double rotation,
		const Zeni::String image)
		: Actor(position, Zeni::Vector2f(64.0f, 32.0f), rotation, image, Zeni::Vector2f(0.0f, 0.0f), Zeni::Vector2f(0.0f, 0.0f), 1.0) {
	m_wheelSeparation = 64.0;
	m_engineForce = 200.0;
	m_tireFriction = 1000.0;
	m_rollingFriction = 50;
	m_braking = false;
	detectCollisionsWithBodies();
	detectCollisionsWithTiles();
	m_startPosition = getPosition();
	m_completedLaps = 0;
	m_driver = nullptr;
}