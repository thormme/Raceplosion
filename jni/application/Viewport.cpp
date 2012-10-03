#include <zenilib.h>
#include "Viewport.h"

Viewport::Viewport(const Zeni::Point2f &viewPosition, const Zeni::Vector2f &viewSize, const Zeni::Point2f &worldViewPosition, const Zeni::Point2f &worldViewSize) {
	m_viewPosition = viewPosition;
	m_viewSize = viewSize;
	m_worldViewPosition = worldViewPosition;
	m_worldViewSize = worldViewSize;
}
Viewport::~Viewport(){}

void Viewport::stepViewportPosition(double timeStep, Zeni::Point2f newPosition) {
	m_worldViewPosition = m_worldViewPosition.interpolate_to(timeStep*5, newPosition);
}
void Viewport::render(const Level &level, const std::vector<Body*> &bodies) const {
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Vector2f screenSize = Zeni::Vector2f(Zeni::Point2f(vr.get_render_target_size()));
	vr.set_2d_view(std::make_pair(m_worldViewPosition, m_worldViewPosition + m_worldViewSize),
		std::make_pair(Zeni::Vector2f(m_viewPosition).multiply_by(screenSize), Zeni::Vector2f(m_viewPosition + m_viewSize).multiply_by(screenSize)), true);

	level.render(m_worldViewPosition, m_worldViewSize);

	for (int i=0; i < bodies.size(); i++) {
		bodies[i]->render();
	}
}

const Zeni::Vector2f Viewport::getWorldViewSize() const {
	return m_worldViewSize;
}