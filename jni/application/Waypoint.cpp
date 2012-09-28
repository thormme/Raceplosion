#include <zenilib.h>
#include "Waypoint.h"

Waypoint::Waypoint(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const double rotation) : Body(position, size, rotation, "waypoint") {
	detectCollisionsWithBodies();
	std::pair<Zeni::Point2f, Zeni::Point2f> bounds = getBoundingBox();
	setPosition(getPosition() + (getPosition() - getCenter()) + (getCenter() - bounds.first));
}