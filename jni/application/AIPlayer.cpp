#include <zenilib.h>
#include "AIPlayer.h"
#include "Utils.h"
#include "NavigationMap.h"
#include "Level.h"

AIPlayer::AIPlayer() {
	m_lookAheadTime = .175 + (double)(rand()%50 - 25)/1000.0; // smaller = understeer, larger = oversteer
	m_directionSmoothing = 2;
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

	/*if (Input::isKeyPressed(SDLK_w)) {
		Rocket * rocket = new Rocket(getCenter(), getRotation(), getVelocity());
		rocket->setPosition(rocket->getPosition() - rocket->getSize()/2.0f);
		stateModifications.bodyAdditions.push_back(rocket);
	}

	if (Input::isKeyDown(SDLK_q)) {
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

void AIPlayer::setNavigationMaps(const std::vector<NavigationMap> &navigationMaps) {
	m_navigationMaps = navigationMaps;
}