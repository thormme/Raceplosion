#include <zenilib.h>
#include "Body.h"
#include "Tile.h"
#include "Level.h"

Body::Body(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const double &rotation,
		const Zeni::String &image,
		const Zeni::Vector2f &velocity,
		const Zeni::Vector2f &force,
		const double &mass)
		:m_position(position), m_size(size), m_rotation(rotation), m_image(image), m_velocity(velocity), m_force(force), m_mass(mass) {
	m_rotationRate = 0;
}

Body::~Body() {
}

void Body::render() {
	render_image(
      m_image, // which texture to use
      m_position, // upper-left corner
      m_position + m_size, // lower-right corner
      -m_rotation, // rotation in radians
      1.0f, // scaling factor
      m_position + 0.5f * m_size, // point to rotate & scale about
      false, // whether or not to horizontally flip the texture
      Zeni::Color()); // what Color to "paint" the texture
}

void Body::stepPhysics(const double timeStep) {
	m_rotation += m_rotationRate * timeStep;
	Zeni::Vector2f acceleration = m_force / m_mass;
	m_velocity += acceleration * timeStep;
	m_position += m_velocity * timeStep;
}

void Body::setPosition(const Zeni::Point2f position) {
	m_position = position;
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

const Zeni::Point2f Body::getPosition() const {
	return m_position;
}

const Zeni::Vector2f Body::getSize() const {
	return m_size;
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
const bool Body::isTouching(const Body &body) const {
	// Radius test
	Zeni::Point2f bodyCenter = body.getSize()/2.0 + body.getPosition();
	Zeni::Point2f center = getSize()/2.0 + getPosition();
	double distance = (center - bodyCenter).magnitude();
	return distance < (body.getSize()/2.0 + getSize()/2.0).magnitude();
}

const bool Body::isTouching(const Zeni::Point2f &position, const Zeni::Vector2f &size) const {
	// Radius bounding box test
	Zeni::Point2f center = getSize()/2.0 + getPosition();
	double boundingRadius = (getSize()/2.0).magnitude();
	if (position.x > center.x + boundingRadius ||
		position.y > center.y + boundingRadius ||
		position.x + size.i < center.x ||
		position.y + size.j < center.y) {
		return false;
	}
	return true;
}

const double Body::getRotation() const {
	return m_rotation;
}