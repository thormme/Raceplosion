#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <zenilib.h>
#include "Level.h"
#include "Body.h"

class Viewport {

private:
	Zeni::Point2f m_viewPosition;
	Zeni::Vector2f m_viewSize;
	Zeni::Point2f m_worldViewPosition;
	Zeni::Vector2f m_worldViewSize;

public:
	Viewport(const Zeni::Point2f &viewPosition, 
			 const Zeni::Vector2f &viewSize, 
			 const Zeni::Point2f &worldViewPosition = Zeni::Point2f(0.0f, 0.0f), 
			 const Zeni::Point2f &worldViewSize = Zeni::Point2f(1200.0f, 1000.0f));
	virtual ~Viewport();

	void stepViewportPosition(double timeStep, Zeni::Point2f newPosition);
	void render(const Level &level, const std::vector<Body*> &bodies) const;

};
#endif