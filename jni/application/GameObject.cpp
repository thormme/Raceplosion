#include <zenilib.h>
#include "GameObject.h"

GameObject::GameObject(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const Zeni::String &image) : m_position(position), m_size(size), m_image(image) {
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
const Zeni::Point2f GameObject::getCenter() const {
	return getSize()/2.0 + getPosition();
}
const Collision GameObject::getCollision(const std::list<Zeni::Point2f> &points, const bool nearest = false) const {
	Collision nearestCollision = Collision();
	Zeni::Vector2f directionVector(cos(getRotation()), sin(getRotation()));
	Zeni::Vector2f perpendicularDirectionVector(cos(getRotation() + Utils::PI/2.0), sin(getRotation() + Utils::PI/2.0));
	double width = getSize().x/2.0;
	double height = getSize().y/2.0;
	for (std::list<Zeni::Point2f>::const_iterator it = points.begin(); it != points.end(); it++) {
		Zeni::Vector2f difference = getCenter() - *it;
		Zeni::Vector2f widthDistanceVector = (difference * directionVector) / directionVector.magnitude() * directionVector;
		if (widthDistanceVector.magnitude() <= width) {
			Zeni::Vector2f heightDistanceVector = (difference * perpendicularDirectionVector) / directionVector.magnitude() * perpendicularDirectionVector;
			if (heightDistanceVector.magnitude() <= height) {
				if (nearest) {
					if (std::min(heightDistanceVector.magnitude2(), widthDistanceVector.magnitude2()) < std::min(nearestCollision.heightDistanceVector.magnitude2(), nearestCollision.widthDistanceVector.magnitude2())) {
						nearestCollision.isColliding = true;
						nearestCollision.heightDistanceVector = heightDistanceVector;
						nearestCollision.widthDistanceVector = widthDistanceVector;
						nearestCollision.position = *it;
					}
				} else {
					nearestCollision.isColliding = true;
					nearestCollision.heightDistanceVector = heightDistanceVector;
					nearestCollision.widthDistanceVector = widthDistanceVector;
					nearestCollision.position = *it;
					return nearestCollision;
				}
			}
		}
	}
	return nearestCollision;
}

GameObject::~GameObject() {
}