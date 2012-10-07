#include <zenilib.h>
#include "Player.h"
#include "Input.h"
#include "Utils.h"

Player::Player() {
	m_controls.resize(FINAL_ELEMENT, InputAction());
	m_raceCar = nullptr;
	m_carImage = "";
}

RaceCar * Player::getNewCar(const Zeni::Point2f &position, const double rotation) {
	if (m_carImage != "") {
		m_raceCar = new RaceCar(position, rotation, m_carImage);
	} else {
		m_raceCar = new RaceCar(position, rotation);
	}
	m_raceCar->m_driver = this;
	return m_raceCar;
}

RaceCar * Player::getLastCar() {
	return m_raceCar;
}

void Player::setControls(const std::vector<InputAction> &controls) {
	m_controls = controls;
	m_controls.resize(FINAL_ELEMENT, InputAction());
}

void Player::setControls(int joyIndex) {
	if (joyIndex >= 0) {
		m_controls[FORWARD] = InputAction(joyIndex, Zeni::Joysticks::AXIS_RIGHT_TRIGGER, 0, 0);
		m_controls[BACKWARD] = InputAction(joyIndex, Zeni::Joysticks::AXIS_LEFT_TRIGGER, 0, 0);
		m_controls[LEFT] = InputAction(joyIndex, Zeni::Joysticks::AXIS_LEFT_THUMB_X, -0.25, 2.0);
		m_controls[RIGHT] = InputAction(joyIndex, Zeni::Joysticks::AXIS_LEFT_THUMB_X, -2.0, 0.25);
		m_controls[FIRE_ROCKET] = InputAction(joyIndex, Zeni::Joysticks::BUTTON_X, InputAction::PRESSED);
		m_controls[LAY_MINE] = InputAction(joyIndex, Zeni::Joysticks::BUTTON_B, InputAction::PRESSED);
		m_controls[RESPAWN] = InputAction(joyIndex, Zeni::Joysticks::BUTTON_BACK, InputAction::PRESSED);
		m_controls[JUMP] = InputAction(joyIndex, Zeni::Joysticks::BUTTON_Y, InputAction::PRESSED);
	}  else {
		m_controls[FORWARD] = InputAction(SDLK_UP);
		m_controls[BACKWARD] = InputAction(SDLK_DOWN);
		m_controls[LEFT] = InputAction(SDLK_LEFT);
		m_controls[RIGHT] = InputAction(SDLK_RIGHT);
		m_controls[FIRE_ROCKET] = InputAction(SDLK_q, InputAction::PRESSED);
		m_controls[LAY_MINE] = InputAction(SDLK_e, InputAction::PRESSED);
		m_controls[RESPAWN] = InputAction(SDLK_BACKSPACE, InputAction::PRESSED);
		m_controls[JUMP] = InputAction(SDLK_w, InputAction::PRESSED);
	}
}

void Player::setCarImage(const Zeni::String &image) {
	m_carImage = image;
}

const Zeni::String Player::getCarImage() const {
	return m_carImage;
}

const StateModifications Player::driveRaceCar(RaceCar &raceCar, const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	raceCar.accelerate(0);
	raceCar.setWheelRotation(0);
	raceCar.setBraking(false);
	if (m_controls[FORWARD].isActive()) {
		raceCar.accelerate(m_controls[FORWARD].getDegree());
		if (raceCar.isMovingInDirection(raceCar.getRotation() + Utils::PI)) {
			raceCar.setBraking(true);
		}
	}
	
	if (m_controls[BACKWARD].isActive()) {
		raceCar.accelerate(-m_controls[BACKWARD].getDegree());
		if (raceCar.isMovingInDirection(raceCar.getRotation())) {
			raceCar.setBraking(true);
		}
	}

	if (m_controls[LEFT].isActive()) {
		raceCar.setWheelRotation(abs(m_controls[LEFT].getDegree())*raceCar.m_maximumWheelRotation);
	}

	if (m_controls[RIGHT].isActive()) {
		raceCar.setWheelRotation(-abs(m_controls[RIGHT].getDegree())*raceCar.m_maximumWheelRotation);
	}

	if (m_controls[FIRE_ROCKET].isActive()) {
		Body * rocket = raceCar.fireRocket();
		if (rocket != nullptr) {
			stateModifications.bodyAdditions.push_back(rocket);
		}
	}

	if (m_controls[LAY_MINE].isActive()) {
		Body * mine = raceCar.layMine();
		if (mine != nullptr) {
			stateModifications.bodyAdditions.push_back(mine);
		}
	}

	if (m_controls[RESPAWN].isActive()) {
		raceCar.respawn();
	}

	if (m_controls[JUMP].isActive()) {
		raceCar.jump();
	}

	/*if (Input::isKeyDown(SDLK_q)) {
		for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
			Tile newTile = Tile((*it)->getPosition(), (*it)->getSize(), Zeni::String("grass"));
			stateModifications.tileChanges.push_back(newTile);
		}
	}*/
	return stateModifications;
}
