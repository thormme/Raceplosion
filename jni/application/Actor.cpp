#include <zenilib.h>
#include "Actor.h"
#include "Input.h"

// Causes the actor to take an action.
void Actor::run() {
	if (Input::isKeyDown(SDLK_DOWN)) {
		setAcceleration(Zeni::Vector2f(100.0f, 100.0f));
	} else {
		setAcceleration(Zeni::Vector2f(0.0f, 0.0f));
	}
}

Actor::Actor(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const double rotation,
		const Zeni::String image,
		const Zeni::Vector2f &velocity,
		const Zeni::Vector2f &acceleration)
		: Body(position, size, rotation, image, velocity, acceleration) {
	m_active = true;
}

void Actor::act() {
	if (m_active) {
		run();
	}
	//m_active = false;
	m_active = true;
}

// If you might delete base class pointers, you need a virtual destructor.
Actor::~Actor() {}