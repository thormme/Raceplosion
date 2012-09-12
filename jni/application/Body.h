#ifndef BODY_H
#define BODY_H
#include <zenilib.h>

class Body {

private:
	Zeni::Point2f position; // Upper left corner
	Zeni::Vector2f velocity; // (width, height)
	Zeni::Vector2f size; // (width, height)
	double rotation;

public:
	Body(const Zeni::Point2f &position,
		  const Zeni::Vector2f &size,
		  const double rotation);
	
	const Zeni::Point2f getPosition();
	const bool isTouching(const Body &body);
	const double getRotation();

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Body();
};
#endif

