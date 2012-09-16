#include <zenilib.h>
#include <cmath>
#include <sstream>
#include "RaceCar.h"
#include "Input.h"

namespace {
	const double PI = 3.141592653589793238462;
}

const Zeni::Vector2f RaceCar::getHorizontalFrictionForce() {
	Zeni::Vector2f directionVector(cos(getRotation()), sin(getRotation()));
	Zeni::Vector2f perpendicularVelocity = getVelocity() - (getVelocity() * directionVector) / directionVector.magnitude() * directionVector;
	return -perpendicularVelocity.normalized()*500/*friction coefficient*/*getMass();
}

// Causes the actor to take an action.
void RaceCar::run() {
	setForce(Zeni::Vector2f(0.0f, 0.0f));
	setRotationRate(0.0);

	if (Input::isKeyDown(SDLK_UP)) {
		setForce(Zeni::Vector2f(cos(getRotation()), sin(getRotation())) * 200);
	}

	if (Input::isKeyDown(SDLK_DOWN)) {
		setForce(Zeni::Vector2f(cos(getRotation()), sin(getRotation())) * -200);
	}

	if (Input::isKeyDown(SDLK_LEFT)) {
		setRotationRate(-2);
	}

	if (Input::isKeyDown(SDLK_RIGHT)) {
		setRotationRate(2);
	}
}

void RaceCar::stepPhysics(const double timeStep) {
	Zeni::Vector2f directionVector(cos(getRotation()), sin(getRotation()));
	Zeni::Vector2f perpendicularVelocity = getVelocity() - (getVelocity() * directionVector) / directionVector.magnitude() * directionVector;
	setForce(getForce()  + getHorizontalFrictionForce());
	/*std::ostringstream str;
	str << perpendicularVelocity.x << " " << perpendicularVelocity.y << "\n";
	OutputDebugString( str.str().c_str());*/
	Actor::stepPhysics(timeStep);
}

RaceCar::RaceCar(const Zeni::Point2f &position,
		const double rotation,
		const Zeni::String image)
		: Actor(position, Zeni::Vector2f(64.0f, 32.0f), rotation, image, Zeni::Vector2f(0.0f, 0.0f), Zeni::Vector2f(0.0f, 0.0f), 1.0) {
}

// If you might delete base class pointers, you need a virtual destructor.
RaceCar::~RaceCar() {}