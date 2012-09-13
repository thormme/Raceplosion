#ifndef ACTOR_H
#define ACTOR_H
#include <zenilib.h>
#include "Body.h"

class Actor : public Body {

private:
	bool m_active;
	
protected:
	// Causes the actor to take an action.
	virtual void run();

public:
	Actor(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f),
		const double rotation = 0.0f,
		const Zeni::String image = "placeholder",
		 const Zeni::Vector2f &velocity = Zeni::Vector2f(0.0f, 0.0f),
		 const Zeni::Vector2f &acceleration = Zeni::Vector2f(0.0f, 0.0f));

	void act();

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Actor();
};
#endif