#include <zenilib.h>
#include "Actor.h"
#include "Input.h"

// Causes the actor to take an action.
const StateModifications Actor::run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications sm = StateModifications();
	sm.bodyAdditions.push_back(new Body());
	return sm;
}

Actor::Actor(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const double rotation,
		const Zeni::String image,
		const Zeni::Vector2f &velocity,
		const Zeni::Vector2f &force,
		const double &mass)
		: Body(position, size, rotation, image, velocity, force, mass) {
	m_active = true;
}

const StateModifications Actor::act(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions) {
	StateModifications stateModifications = StateModifications();
	if (m_active) {
		stateModifications = run(tileCollisions, bodyCollisions);
	}
	//m_active = false;
	m_active = true;

	return stateModifications;
}