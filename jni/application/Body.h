#ifndef BODY_H
#define BODY_H
#include <zenilib.h>

class Body {

private:
	Zeni::Point2f m_position; // Upper left corner
	Zeni::Vector2f m_size; // (width, height)
	double m_rotation; // Rotation of the Body
	Zeni::String m_image; // Name of the image to draw
	Zeni::Vector2f m_velocity;
	Zeni::Vector2f m_acceleration;

	double m_rotationRate; // Radians per second

public:
	Body(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		 const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f),
		 const double rotation = 0.0f,
		 const Zeni::String image = "placeholder",
		 const Zeni::Vector2f &velocity = Zeni::Vector2f(0.0f, 0.0f),
		 const Zeni::Vector2f &acceleration = Zeni::Vector2f(0.0f, 0.0f));

	// Render the body.
	void render();
	// Run the physics simulation on this object for one step.
	virtual void stepPhysics(const double timeStep);

	void setPosition(Zeni::Point2f position);
	void setVelocity(Zeni::Vector2f velocity);
	void setAcceleration(Zeni::Vector2f acceleration);
	void setRotation(double rotation);
	void setRotationRate(double rotationRate);
	
	const Zeni::Point2f getPosition();
	const Zeni::Vector2f Body::getVelocity();
	const Zeni::Vector2f Body::getAcceleration();
	const bool isTouching(const Body &body);
	const double getRotation();

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Body();
};
#endif

