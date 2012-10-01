#include <zenilib.h>
#include "AIRaceCar.h"
#include "Utils.h"
#include "NavigationMap.h"
#include "Level.h"

const StateModifications AIRaceCar::run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	setForce(Zeni::Vector2f(0.0f, 0.0f));
	setWheelRotation(0);
	setBraking(false);
	accelerate(1.0);

	/*if (Input::isKeyDown(SDLK_DOWN)) {
		accelerate(-1.0);
		if (isMovingInDirection(getRotation())) {
			setBraking(true);
		}
	}*/
	Level::waypoint = getPassedWaypoints().size()%m_navigationMaps.size();
	Utils::printDebugMessage(Level::waypoint);
	Utils::printDebugMessage("-\n");
	if (Utils::getAngleDifference(m_navigationMaps[Level::waypoint].getSuggestedDirectionAtPosition(getCenter()), getRotation()) > .1) {
		setWheelRotation(Utils::PI/6);
	}

	if (Utils::getAngleDifference(m_navigationMaps[Level::waypoint].getSuggestedDirectionAtPosition(getCenter()), getRotation()) < -.1) {
		setWheelRotation(-Utils::PI/6);
	}

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
	return stateModifications;
}

void AIRaceCar::setNavigationMaps(const std::vector<NavigationMap> &navigationMaps) {
	m_navigationMaps = navigationMaps;
}

AIRaceCar::AIRaceCar(const Zeni::Point2f &position,
		const double rotation,
		const Zeni::String image) : RaceCar(position, rotation, image) {
}