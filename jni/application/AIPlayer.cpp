#include <zenilib.h>
#include "AIPlayer.h"
#include "Utils.h"
#include "NavigationMap.h"
#include "Level.h"
#include "Time.h"

AIPlayer::AIPlayer() {
	m_lookAheadTime = .175 + (double)(rand()%50 - 25)/1000.0; // smaller = understeer, larger = oversteer
	m_directionSmoothing = 2;
	m_fireRocket = false;
	m_layMine = false;
	m_rocketTimer = 0;
	m_mineTimer = 0;
}

const StateModifications AIPlayer::driveRaceCar(RaceCar &raceCar, const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	raceCar.setForce(Zeni::Vector2f(0.0f, 0.0f));
	raceCar.setWheelRotation(0);
	raceCar.setBraking(false);
	raceCar.accelerate(1.0);

	/*if (Input::isKeyDown(SDLK_DOWN)) {
		accelerate(-1.0);
		if (isMovingInDirection(getRotation())) {
			setBraking(true);
		}
	}*/
	int waypoint = raceCar.getPassedWaypoints().size()%m_navigationMaps.size();
	Level::waypoint = waypoint; // TODO: Remove or move/hide debug functionality
	double suggestedDirection = m_navigationMaps[waypoint].getSuggestedDirectionAtPosition(raceCar.getCenter()+raceCar.getVelocity()*m_lookAheadTime, m_directionSmoothing);
	double angleDifference = Utils::getAngleDifference(suggestedDirection, raceCar.getRotation());
	if (angleDifference < -raceCar.m_maximumWheelRotation) {
		angleDifference = -raceCar.m_maximumWheelRotation;
	}
	if (angleDifference > raceCar.m_maximumWheelRotation) {
		angleDifference = raceCar.m_maximumWheelRotation;
	}
	raceCar.setWheelRotation(angleDifference);

	for (int i=0; i < bodyCollisions.size(); i++) {
		RaceCar * car = dynamic_cast<RaceCar*>(bodyCollisions[i]);
		if (car != nullptr) {
			m_fireRocket = false;
			m_layMine = false;
		}
	}

	if (m_fireRocket == true) {
		m_rocketTimer = Time::getGameTime();
		Body * rocket = raceCar.fireRocket();
		if (rocket != nullptr) {
			stateModifications.bodyAdditions.push_back(rocket);
		}
		m_fireRocket = false;
	}

	if (m_layMine == true) {
		m_mineTimer = Time::getGameTime();
		Body * mine = raceCar.layMine();
		if (mine != nullptr) {
			stateModifications.bodyAdditions.push_back(mine);
		}
		m_layMine = false;
	}

	/*if (Input::isKeyDown(SDLK_q)) {
		for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
			Tile newTile = Tile((*it)->getPosition(), (*it)->getSize(), Zeni::String("grass"));
			stateModifications.tileChanges.push_back(newTile);
		}
	}*/

	if (raceCar.getHealth().remaining == 0) {
		raceCar.respawn();
	}

	return stateModifications;
}

void AIPlayer::senseSurroundings(std::vector<Tile*> tiles, std::vector<Body*> bodies) {
	for (int i=0; i < bodies.size(); i++) {
		RaceCar * car = dynamic_cast<RaceCar*>(bodies[i]);
		if (Time::getGameTime() - m_rocketTimer > .3 &&
			car != nullptr && 
			(car->getPosition() - getLastCar()->getPosition()).magnitude() > 100.0f) {
			if (std::abs(Utils::getAngleDifference(Utils::getAngleFromVector(car->getCenter() - getLastCar()->getCenter()), getLastCar()->getRotation())) < Utils::PI/9.0) {
				m_fireRocket = true;
			} else if (std::abs(Utils::getAngleDifference(Utils::getAngleFromVector(car->getCenter() - getLastCar()->getCenter()), getLastCar()->getRotation())) > 8.0*Utils::PI/9.0) {
				m_layMine = true;
			}
		}
	}
}

void AIPlayer::setNavigationMaps(const std::vector<NavigationMap> &navigationMaps) {
	m_navigationMaps = navigationMaps;
}