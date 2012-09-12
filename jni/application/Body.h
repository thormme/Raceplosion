#ifndef BODY_H
#define BODY_H
#include <zenilib.h>

class Body {

private:
	Zeni::Point2f m_position; // Upper left corner
	Zeni::Vector2f m_velocity; // (width, height)
	Zeni::Vector2f m_size; // (width, height)
	double m_rotation; // Rotation of the Body
	String m_image; // Name of the image to draw

public:
	Body(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		 const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f),
		 const double rotation = 0.0f,
		 const String image = "placeholder");

	void draw();
	
	const Zeni::Point2f getPosition();
	const bool isTouching(const Body &body);
	const double getRotation();

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Body();
};
#endif

