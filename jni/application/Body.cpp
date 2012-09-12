#include "Body.h"

Body::Body(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const double rotation):m_position(position), m_size(size), m_rotation(rotation) {

}

Body::~Body() {
}

const Zeni::Point2f Body::getPosition() {
	return m_position;
}

const bool Body::isTouching(const Body &body) {
	return false;
}

const double Body::getRotation() {
	return m_rotation;
}