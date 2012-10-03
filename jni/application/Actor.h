#ifndef ACTOR_H
#define ACTOR_H
#include <zenilib.h>
#include "Body.h"

struct StateModifications {
	std::list<Tile> tileChanges;
	std::list<Body*> bodyAdditions;
	std::list<Body*> bodyRemovals;

	void combine(const StateModifications &stateModifications) {
		tileChanges.insert(tileChanges.end(), stateModifications.tileChanges.begin(), stateModifications.tileChanges.end());
		bodyAdditions.insert(bodyAdditions.end(), stateModifications.bodyAdditions.begin(), stateModifications.bodyAdditions.end());
		bodyRemovals.insert(bodyRemovals.end(), stateModifications.bodyRemovals.begin(), stateModifications.bodyRemovals.end());
	}
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

	void setActive(bool active);
};
#endif