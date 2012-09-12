#ifndef ACTOR_H
#define ACTOR_H
#include <zenilib.h>
#include "Body.h"

class Actor : Body {

private:
	bool m_active;
	
	// Causes the actor to take an action.
	void run();

public:
	Actor(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f),
		const double rotation = 0.0f,
		const String image = "placeholder");

	void act();

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Actor();
};
#endif