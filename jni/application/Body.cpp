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
	m_detectCollisionsWithTiles = false;
	m_detectCollisionsWithBodies = false;
}

Body::~Body() {
}

void Body::render() {
	Zeni::Point2f center = getSize()/2.0 + getPosition();
	double boundingRadius = (getSize()/2.0).magnitude();
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Vertex2f_Color v0(Zeni::Point2f(center.x - boundingRadius,center.y - boundingRadius), Zeni::Color(1.f,1.f,0.f,0.f));
    Zeni::Vertex2f_Color v1(Zeni::Point2f(center.x - boundingRadius,center.y + boundingRadius), Zeni::get_Colors()["blue"]);
    Zeni::Vertex2f_Color v2(Zeni::Point2f(center.x + boundingRadius,center.y + boundingRadius), Zeni::get_Colors()["green"]);
    Zeni::Vertex2f_Color v3(Zeni::Point2f(center.x + boundingRadius,center.y - boundingRadius), Zeni::Color());

    Zeni::Quadrilateral<Zeni::Vertex2f_Color> quad(v0, v1, v2, v3);
    vr.render(quad);
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

void Body::handleCollisions(const double timeStep, std::vector<Tile*> tiles, std::vector<Body*> bodies) {
	// TODO: implement
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

void Body::detectCollisionsWithTiles() {
	m_detectCollisionsWithTiles = true;
}

void Body::detectCollisionsWithBodies() {
	m_detectCollisionsWithBodies = true;
}

const Zeni::Point2f Body::getPosition() const {
	return m_position;
}

const Zeni::Vector2f Body::getSize() const {
	return m_size;
}

const std::pair<Zeni::Point2f, Zeni::Point2f> Body::getBoundingBox() const {
	Zeni::Point2f center = getCenter();
	double boundingRadius = (getSize()/2.0).magnitude();
	return std::make_pair(Zeni::Point2f(center.x - boundingRadius, center.y - boundingRadius),
				   Zeni::Point2f(center.x + boundingRadius, center.y + boundingRadius));
}

const Zeni::Point2f Body::getCenter() const {
	return getSize()/2.0 + getPosition();
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
	Zeni::Point2f bodyCenter = body.getCenter();
	Zeni::Point2f center = getCenter();
	double distance = (center - bodyCenter).magnitude();
	return distance < (body.getSize()/2.0 + getSize()/2.0).magnitude();
}

// TODO: implement
const bool Body::isTouching(const Zeni::Point2f &position, const Zeni::Vector2f &size) const {
	// Radius bounding box test
	std::pair<Zeni::Point2f, Zeni::Point2f> boundingBox = getBoundingBox();
	if (position.x > boundingBox.second.x ||
		position.y > boundingBox.second.y ||
		position.x + size.i < boundingBox.first.x ||
		position.y + size.j < boundingBox.first.y) {
		return false;
	}
	return true;
}

const double Body::getRotation() const {
	return m_rotation;
}

const bool Body::willDetectCollisionsWithTiles() const {
	return m_detectCollisionsWithTiles;
}

const bool Body::willDetectCollisionsWithBodies() const {
	return m_detectCollisionsWithBodies;
}