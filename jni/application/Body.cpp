#include <zenilib.h>
#include <algorithm>
#include "Utils.h"
#include "Body.h"
#include "Tile.h"

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
	/*std::pair<Zeni::Point2f, Zeni::Point2f> boundingBox = getBoundingBox();
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Vertex2f_Color v0(boundingBox.first, Zeni::Color(1.f,1.f,0.f,0.f));
    Zeni::Vertex2f_Color v1(Zeni::Point2f(boundingBox.first.x, boundingBox.second.y), Zeni::get_Colors()["blue"]);
    Zeni::Vertex2f_Color v2(boundingBox.second, Zeni::get_Colors()["green"]);
    Zeni::Vertex2f_Color v3(Zeni::Point2f(boundingBox.second.x, boundingBox.first.y), Zeni::Color());

    Zeni::Quadrilateral<Zeni::Vertex2f_Color> quad(v0, v1, v2, v3);
    vr.render(quad);*/
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
	Zeni::Vector2f halfSize = getSize()/2.0;
	double rotationSin = abs(sin(getRotation()));
	double rotationCos = abs(cos(getRotation()));
	double boundingX = rotationCos*halfSize.x + rotationSin*halfSize.y;
	double boundingY = rotationSin*halfSize.x + rotationCos*halfSize.y;
	return std::make_pair(Zeni::Point2f(center.x - boundingX, center.y - boundingY),
						  Zeni::Point2f(center.x + boundingX, center.y + boundingY));
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

// TODO: improve
const bool Body::isTouching(const Zeni::Point2f &position, const Zeni::Vector2f &size) const {
	// Radius bounding box test
	std::pair<Zeni::Point2f, Zeni::Point2f> boundingBox = getBoundingBox();
	if (position.x > boundingBox.second.x ||
		position.y > boundingBox.second.y ||
		position.x + size.i < boundingBox.first.x ||
		position.y + size.j < boundingBox.first.y) {
		return false;
	}
	// Test whether corners intersect body
	std::list<Zeni::Point2f> points;
	points.push_back(position);
	points.push_back(position + size);
	points.push_back(Zeni::Point2f(position.x + size.x, position.y));
	points.push_back(Zeni::Point2f(position.x, position.y + size.y));
	Zeni::Vector2f directionVector(cos(getRotation()), sin(getRotation()));
	Zeni::Vector2f perpendicularDirectionVector(cos(getRotation() + Utils::PI/2.0), sin(getRotation() + Utils::PI/2.0));
	double width = getSize().x/2.0;
	double height = getSize().y/2.0;
	for (std::list<Zeni::Point2f>::iterator it = points.begin(); it != points.end(); it++) {
		Zeni::Vector2f difference = getCenter() - *it;
		Zeni::Vector2f widthDistanceVector = (difference * directionVector) / directionVector.magnitude() * directionVector;
		if (widthDistanceVector.magnitude() <= width) {
			Zeni::Vector2f heightDistanceVector = (difference * perpendicularDirectionVector) / directionVector.magnitude() * perpendicularDirectionVector;
			if (heightDistanceVector.magnitude() <= height) {
				return true;
			}
		}
	}
	return false;
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