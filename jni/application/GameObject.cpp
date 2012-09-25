#include <zenilib.h>
#include "GameObject.h"

GameObject::GameObject(const Zeni::Point2f &position,
		const Zeni::Vector2f &size) : m_position(position), m_size(size) {
}

void GameObject::setPosition(const Zeni::Point2f position) {
	m_position = position;
}
	
const Zeni::Point2f GameObject::getPosition() const {
	return m_position;
}
const Zeni::Vector2f GameObject::getSize() const {
	return m_size;
}
const std::pair<Zeni::Point2f, Zeni::Point2f> GameObject::getBoundingBox() const {
	return std::make_pair(getPosition(), getPosition() + getSize());
}
const std::list<Zeni::Point2f> GameObject::getBoundingPoints() const {
	Zeni::Point2f position = getPosition();
	Zeni::Point2f size = getSize();
	std::list<Zeni::Point2f> points;
	points.push_back(position);
	points.push_back(position + size);
	points.push_back(Zeni::Point2f(position.x + size.x, position.y));
	points.push_back(Zeni::Point2f(position.x, position.y + size.y));
	return points;
}
const Zeni::Point2f GameObject::getCenter() const {
	return getSize()/2.0 + getPosition();
}
const Collision GameObject::getCollision(const std::list<Zeni::Point2f> &points, const bool nearest) const {
	Collision nearestCollision = Collision();
	std::pair<Zeni::Point2f, Zeni::Point2f> boundingBox = getBoundingBox();
	for (std::list<Zeni::Point2f>::const_iterator it = points.begin(); it != points.end(); it++) {
		// Radius bounding box test
		if (it->x < boundingBox.second.x &&
			it->y < boundingBox.second.y &&
			it->x > boundingBox.first.x &&
			it->y > boundingBox.first.y) {
			
			double leftXDiff = it->x - boundingBox.first.x;
			double topYDiff = it->y - boundingBox.first.y;
			double rightXDiff = it->x - boundingBox.second.x;
			double bottomYDiff = it->y - boundingBox.second.y;
			double minimumX = abs(leftXDiff) < abs(rightXDiff) ? leftXDiff : rightXDiff;
			double minimumY = abs(topYDiff) < abs(bottomYDiff) ? topYDiff : bottomYDiff;
			Zeni::Vector2f minDifferenceVector;
			if (abs(minimumY) < abs(minimumX)) {
				minDifferenceVector = Zeni::Vector2f(0.0f, minimumY);
			} else {
				minDifferenceVector = Zeni::Vector2f(minimumX, 0.0f);
			}
			if (nearest) {
				if (minDifferenceVector.magnitude2() < nearestCollision.minimumSeparation.magnitude2()) {
					nearestCollision.isColliding = true;
					nearestCollision.position = *it;
					nearestCollision.minimumSeparation = minDifferenceVector;
				}
			} else {
				nearestCollision.isColliding = true;
				nearestCollision.position = *it;
				nearestCollision.minimumSeparation = minDifferenceVector;
				return nearestCollision;
			}
		}
	}
	return nearestCollision;
}

GameObject::~GameObject() {
}