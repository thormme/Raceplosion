#ifndef BODY_H
#define BODY_H
#include <zenilib.h>
#include "Tile.h"

class Body {

private:
	Zeni::Point2f m_position; // Upper left corner
	Zeni::Vector2f m_size; // (width, height)
	double m_rotation; // Rotation of the Body
	Zeni::String m_image; // Name of the image to draw
	Zeni::Vector2f m_velocity;
	Zeni::Vector2f m_force;
	double m_mass;

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

	void setPosition(const Zeni::Point2f position);
	void setVelocity(const Zeni::Vector2f velocity);
	void setAcceleration(const Zeni::Vector2f acceleration);
	void setForce(const Zeni::Vector2f force);
	void setRotation(const double rotation);
	void setRotationRate(const double rotationRate);
	
	const Zeni::Point2f getPosition() const;
	const Zeni::Vector2f getSize() const;
	const Zeni::Vector2f getVelocity() const;
	const Zeni::Vector2f getAcceleration() const;
	const Zeni::Vector2f getForce() const;
	const double getMass() const;
	const bool isTouching(const Body &body) const;
	const bool isTouching(const Zeni::Point2f &position, const Zeni::Vector2f &size) const;
	const double getRotation() const;

	virtual ~Body();
};
#endif

