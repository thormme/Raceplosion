#include <zenilib.h>
#include "Player.h"
#include "Input.h"
#include "Utils.h"

Player::Player() {
	/*m_forward = InputAction(0, Zeni::Joysticks::AXIS_RIGHT_TRIGGER, 0, 0);
	m_backward = InputAction(0, Zeni::Joysticks::AXIS_LEFT_TRIGGER, 0, 0);
	m_left = InputAction(0, Zeni::Joysticks::AXIS_LEFT_THUMB_X, -0.25, 2.0);
	m_right = InputAction(0, Zeni::Joysticks::AXIS_LEFT_THUMB_X, -2.0, 0.25);*/

	m_forward = InputAction(SDLK_UP);
	m_backward = InputAction(SDLK_DOWN);
	m_left = InputAction(SDLK_LEFT);
	m_right = InputAction(SDLK_RIGHT);
}

RaceCar * Player::getNewCar(const Zeni::Point2f &position, const double rotation) {
	m_raceCar = new RaceCar(position, rotation);
	m_raceCar->m_driver = this;
	return m_raceCar;
}

RaceCar * Player::getLastCar() {
	return m_raceCar;
}

const StateModifications Player::driveRaceCar(RaceCar &raceCar, const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	raceCar.accelerate(0);
	raceCar.setWheelRotation(0);
	raceCar.setBraking(false);
	if (m_forward.isActive()) {
		raceCar.accelerate(m_forward.getDegree());
		if (raceCar.isMovingInDirection(raceCar.getRotation() + Utils::PI)) {
			raceCar.setBraking(true);
		}
	}
	
	if (m_backward.isActive()) {
		raceCar.accelerate(-m_backward.getDegree());
		if (raceCar.isMovingInDirection(raceCar.getRotation())) {
			raceCar.setBraking(true);
		}
	}

	if (m_left.isActive()) {
		raceCar.setWheelRotation(abs(m_left.getDegree())*Utils::PI/6);
	}

	if (m_right.isActive()) {
		raceCar.setWheelRotation(-abs(m_right.getDegree())*Utils::PI/6);
	}

	if (Input::isKeyPressed(SDLK_w)) {
		stateModifications.bodyAdditions.push_back(raceCar.fireRocket());
	}

	if (Input::isKeyDown(SDLK_q)) {
		for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
			Tile newTile = Tile((*it)->getPosition(), (*it)->getSize(), Zeni::String("grass"));
			stateModifications.tileChanges.push_back(newTile);
		}
	}
	return stateModifications;
}