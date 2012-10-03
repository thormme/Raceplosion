#ifndef RACE_CAR_VIEWPORT_H
#define RACE_CAR_VIEWPORT_H
#include <zenilib.h>
#include "Viewport.h"
#include "Level.h"
#include "Body.h"
#include "RaceCar.h"

class RaceCarViewport : public Viewport {

private:
	const RaceCar * m_raceCar;

public:
	RaceCarViewport(const RaceCar * raceCar, 
			const Zeni::Point2f &viewPosition, 
			const Zeni::Vector2f &viewSize, 
			const Zeni::Point2f &worldViewPosition = Zeni::Point2f(0.0f, 0.0f), 
			const Zeni::Point2f &worldViewSize = Zeni::Point2f(1200.0f, 1000.0f));

	virtual ~RaceCarViewport();

	void render(const Level &level, const std::vector<Body*> &bodies) const;

};
#endif