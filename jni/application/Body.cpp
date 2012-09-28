#include <zenilib.h>
#include <algorithm>
#include "Utils.h"
#include "Body.h"
#include "GameObject.h"
#include "Tile.h"

Body::Body(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const double &rotation,
		const Zeni::String &image,
		const Zeni::Vector2f &velocity,
		const Zeni::Vector2f &force,
		const double &mass)
		:GameObject(position, size), m_rotation(rotation), m_image(image), m_velocity(velocity), m_force(force), m_mass(mass) {
	m_rotationRate = 0;
	m_detectCollisionsWithTiles = false;
	m_detectCollisionsWithBodies = false;
}

void Body::render() {
	render_image(
      m_image, // which texture to use
      getPosition(), // upper-left corner
      getPosition() + getSize(), // lower-right corner
      -m_rotation, // rotation in radians
      1.0f, // scaling factor
      getPosition() + 0.5f * getSize(), // point to rotate & scale about
      false, // whether or not to horizontally flip the texture
      Zeni::Color()); // what Color to "paint" the texture
}

void Body::stepPhysics(const double timeStep) {
	m_rotation += m_rotationRate * timeStep;
	Zeni::Vector2f acceleration = m_force / m_mass;
	m_velocity += acceleration * timeStep;
	setPosition(getPosition() + m_velocity * timeStep);
}

void Body::handleCollisions(const double timeStep, std::vector<Tile*> tiles, std::vector<Body*> bodies) {
	// TODO: implement
}

void Body::setVelocity(const Zeni::Vector2f velocity) {
	m_velocity = velocity;
}

void Body::setForce(const Zeni::Vector2f force) {
	m_force = force;
}

void Body::setRotation(const double rotation) {
	m_rotation = rotation;
}

void Body::setRotationRate(const double rotationRate) {
	m_rotationRate = rotationRate;
}

void Body::detectCollisionsWithTiles() {
	m_detectCollisionsWithTiles = true;
}

void Body::detectCollisionsWithBodies() {
	m_detectCollisionsWithBodies = true;
}

const std::pair<Zeni::Point2f, Zeni::Point2f> Body::getBoundingBox() const {
	Zeni::Point2f center = getCenter();
	Zeni::Vector2f halfSize = getSize()/2.0;
	double rotationSin = abs(sin(getRotation()));
	double rotationCos = abs(cos(getRotation()));
	double boundingX = rotationCos*halfSize.x + rotationSin*halfSize.y;
	double boundingY = rotationSin*halfSize.x + rotationCos*halfSize.y;
	return std::make_pair(Zeni::Point2f(center.x - boundingX, center.y - boundingY),
						  Zeni::Point2f(center.x + boundingX, center.y + boundingY));
}

const std::list<Zeni::Point2f> Body::getBoundingPoints() const {
	Zeni::Point2f position = getPosition();
	Zeni::Point2f size = getSize();
	Zeni::Vector2f perpendicularVector = Utils::getVectorFromAngle(getRotation() + Utils::PI/2.0) * getSize().j/2.0f;
	Zeni::Vector2f parallelVector = getRotationVector() * getSize().i/2.0f;
	std::list<Zeni::Point2f> points;
	points.push_back(perpendicularVector + parallelVector + getCenter());
	points.push_back(perpendicularVector - parallelVector + getCenter());
	points.push_back(-perpendicularVector + parallelVector + getCenter());
	points.push_back(-perpendicularVector - parallelVector + getCenter());
	return points;
}

const Zeni::Vector2f Body::getVelocity() const {
	return m_velocity;
}

const Zeni::Vector2f Body::getForce() const {
	return m_force;
}

const double Body::getMass() const {
	return m_mass;
}

// TODO: implement
const bool Body::isTouching(const GameObject &object) const {
	// Radius test
	std::pair<Zeni::Point2f, Zeni::Point2f> thisBoundingBox = getBoundingBox();
	std::pair<Zeni::Point2f, Zeni::Point2f> otherBoundingBox = object.getBoundingBox();
	if (otherBoundingBox.first.x > thisBoundingBox.second.x ||
		otherBoundingBox.first.y > thisBoundingBox.second.y ||
		otherBoundingBox.second.x < thisBoundingBox.first.x ||
		otherBoundingBox.second.y < thisBoundingBox.first.y) {
		return false;
	}
	// Test whether corners intersect body
	return getCollision(object.getBoundingPoints()).isColliding || object.getCollision(getBoundingPoints()).isColliding;
}

const Collision Body::getCollision(const std::list<Zeni::Point2f> &points, const bool nearest) const {
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
				double heightDifference = getSize().j/2.0 - heightDistanceVector.magnitude();
				double widthDifference = getSize().i/2.0 - widthDistanceVector.magnitude();
				Zeni::Vector2f minimumSeparation = heightDifference < widthDifference ? heightDistanceVector.normalized()*heightDifference : widthDistanceVector.normalized()*widthDifference;
				if (nearest) {
					if (minimumSeparation.magnitude2() < nearestCollision.minimumSeparation.magnitude2()) {
						nearestCollision.isColliding = true;
						nearestCollision.minimumSeparation = minimumSeparation;
						nearestCollision.position = *it;
					}
				} else {
					nearestCollision.isColliding = true;
					nearestCollision.minimumSeparation = minimumSeparation;
					nearestCollision.position = *it;
					return nearestCollision;
				}
			}
		}
	}
	return nearestCollision;
}

const double Body::getRotation() const {
	return m_rotation;
}

const Zeni::Vector2f Body::getRotationVector() const {
	return Zeni::Vector2f(cos(getRotation()), sin(getRotation())); 
}

const bool Body::willDetectCollisionsWithTiles() const {
	return m_detectCollisionsWithTiles;
}

const bool Body::willDetectCollisionsWithBodies() const {
	return m_detectCollisionsWithBodies;
}