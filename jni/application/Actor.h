#ifndef ACTOR_H
#define ACTOR_H
#include <zenilib.h>
#include "Body.h"

struct StateModifications {
	std::vector<Tile*> tileChanges;
	std::vector<Body*> bodyAdditions;
	std::vector<Body*> bodyRemovals;
};

class Actor : public Body {

private:
	bool m_active;
	
protected:

	virtual const StateModifications run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions); ///< Causes the actor to take an action.

public:
	Actor(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		 const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f),
		 const double rotation = 0.0f,
		 const Zeni::String image = "placeholder",
		 const Zeni::Vector2f &velocity = Zeni::Vector2f(0.0f, 0.0f),
		 const Zeni::Vector2f &acceleration = Zeni::Vector2f(0.0f, 0.0f),
		 const double &mass = 1.0);

	const StateModifications act(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);

	virtual ~Actor();
};
#endif