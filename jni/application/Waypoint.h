#ifndef WAYPOINT_H
#define WAYPOINT_H
#include <zenilib.h>
#include "Actor.h"

class Waypoint : public Body {

private:

public:
	Waypoint(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const Zeni::Vector2f &size = Zeni::Vector2f(256.0f, 32.0f),
		const double rotation = 0);

};
#endif