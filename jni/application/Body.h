#ifndef BODY_H
#define BODY_H
#include <zenilib.h>
#include <limits>
#include "Tile.h"

class Body : public GameObject {

private:
	double m_rotation; // Rotation of the Body
	Zeni::String m_image; // Name of the image to draw
	Zeni::Vector2f m_velocity;
	Zeni::Vector2f m_force;
	double m_mass;
	bool m_detectCollisionsWithTiles;
	bool m_detectCollisionsWithBodies;

	double m_rotationRate; // Radians per second

public:
	Body(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		 const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f),
		 const double &rotation = 0.0,
		 const Zeni::String &image = "placeholder",
		 const Zeni::Vector2f &velocity = Zeni::Vector2f(0.0f, 0.0f),
		 const Zeni::Vector2f &acceleration = Zeni::Vector2f(0.0f, 0.0f),
		 const double &mass = 1.0);

	
	void render(); ///< Render the body.
	
	virtual void stepPhysics(const double timeStep); ///< Run the physics simulation on this object for one step.
	virtual void handleCollisions(const double timeStep, std::vector<Tile*> tiles, std::vector<Body*> bodies); ///< Run physics necessary to deal with collisions.

	void setVelocity(const Zeni::Vector2f velocity);
	void setAcceleration(const Zeni::Vector2f acceleration);
	void setForce(const Zeni::Vector2f force);
	void setRotation(const double rotation);
	void setRotationRate(const double rotationRate);

	void detectCollisionsWithTiles();
	void detectCollisionsWithBodies();
	
	const std::pair<Zeni::Point2f, Zeni::Point2f> getBoundingBox() const;
	const std::list<Zeni::Point2f> getBoundingPoints() const;
	const Zeni::Vector2f getVelocity() const;
	const Zeni::Vector2f getAcceleration() const;
	const Zeni::Vector2f getForce() const;
	const double getMass() const;
	const bool isTouching(const GameObject &object) const;
	const Collision getCollision(const std::list<Zeni::Point2f> &points, const bool nearest = false) const;
	const double getRotation() const;
	const Zeni::Vector2f getRotationVector() const;
	const bool willDetectCollisionsWithTiles() const;
	const bool willDetectCollisionsWithBodies() const;

	//virtual ~Body();
};
#endif

