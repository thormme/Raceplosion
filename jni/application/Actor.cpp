#include <zenilib.h>
#include "Actor.h"

// Causes the actor to take an action.
void Actor::run() {
		
}

Actor::Actor(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const double rotation)
		: Body(position, size, rotation) {
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