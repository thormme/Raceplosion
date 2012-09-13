#include "Body.h"

Body::Body(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const double rotation,
		const Zeni::String image,
		const Zeni::Vector2f &velocity,
		const Zeni::Vector2f &acceleration)
		:m_position(position), m_size(size), m_rotation(rotation), m_image(image), m_velocity(position), m_acceleration(position) {
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
	m_velocity += m_acceleration * timeStep;
	m_position += m_velocity * timeStep;
}

void Body::setPosition(Zeni::Point2f position) {
	m_position = position;
}

void Body::setVelocity(Zeni::Vector2f velocity) {
	m_velocity = velocity;
}

void Body::setAcceleration(Zeni::Vector2f acceleration) {
	m_acceleration = acceleration;
}

void Body::setRotation(double rotation) {
	m_rotation = rotation;
}

void Body::setRotationRate(double rotationRate) {
	m_rotationRate = rotationRate;
}

const Zeni::Point2f Body::getPosition() {
	return m_position;
}

const Zeni::Vector2f Body::getVelocity() {
	return m_velocity;
}

const Zeni::Vector2f Body::getAcceleration() {
	return m_acceleration;
}

// TODO: implement
const bool Body::isTouching(const Body &body) {
	return false;
}

const double Body::getRotation() {
	return m_rotation;
}