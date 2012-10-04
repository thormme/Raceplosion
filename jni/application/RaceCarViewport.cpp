#include <zenilib.h>
#include <sstream>
#include "RaceCarViewport.h"
#include "Viewport.h"
#include "RaceCar.h"

RaceCarViewport::RaceCarViewport(const RaceCar * raceCar, 
			const Zeni::Point2f &viewPosition, 
			const Zeni::Vector2f &viewSize, 
			const Zeni::Point2f &worldViewPosition, 
			const Zeni::Point2f &worldViewSize) 
			: m_raceCar(raceCar), Viewport(viewPosition, viewSize, worldViewPosition, worldViewSize) {
}

RaceCarViewport::~RaceCarViewport() {}

void RaceCarViewport::render(const Level &level, const std::vector<Body*> &bodies) const {
	float healthX = 0.0f;
	float rocketX = healthX + 78.0f + 32.0f;
	float jumpX = rocketX + 78.0f + 32.0f;
	float mineX = jumpX + 78.0f + 32.0f;

	// Render scene
	Viewport::render(level, bodies);
	// Render HUD
	Zeni::Vector2f hudPosition = Zeni::Vector2f(m_worldViewSize.i - 420.0, 0);
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Vector2f screenSize = Zeni::Vector2f(Zeni::Point2f(vr.get_render_target_size()));
	vr.set_2d_view(std::make_pair(Zeni::Point2f(0.0f, 0.0f), m_worldViewSize),
		std::make_pair(Zeni::Vector2f(m_viewPosition).multiply_by(screenSize), Zeni::Vector2f(m_viewPosition + m_viewSize).multiply_by(screenSize)), true);
	Zeni::Vertex2f_Texture ul(Zeni::Point2f(healthX,  0.0f) + hudPosition, Zeni::Point2f(0.0f, 0.0f));
	Zeni::Vertex2f_Texture ll(Zeni::Point2f(healthX, 32.0f) + hudPosition, Zeni::Point2f(0.0f, 1.0f));
	Zeni::Vertex2f_Texture lr(Zeni::Point2f(healthX + 32.0f, 32.0f) + hudPosition, Zeni::Point2f(1.0f, 1.0f));
	Zeni::Vertex2f_Texture ur(Zeni::Point2f(healthX + 32.0f,  0.0f) + hudPosition, Zeni::Point2f(1.0f, 0.0f));
	Zeni::Material material("health");
	Zeni::Quadrilateral<Zeni::Vertex2f_Texture> quad(ul, ll, lr, ur);
	quad.fax_Material(&material);
	vr.render(quad);

	std::ostringstream str;
	str << m_raceCar->getHealth().remaining;
	Zeni::get_Fonts()["silkscreen_normal"].render_text(Zeni::String(str.str()), Zeni::Point2f(32, -6) + hudPosition, Zeni::Color(0xFFFFFFFF));

	ul = Zeni::Vertex2f_Texture(Zeni::Point2f(rocketX, 12.0f) + hudPosition, Zeni::Point2f(0.0f, 0.0f));
	ll = Zeni::Vertex2f_Texture(Zeni::Point2f(rocketX, 20.0f) + hudPosition, Zeni::Point2f(0.0f, 1.0f));
	lr = Zeni::Vertex2f_Texture(Zeni::Point2f(rocketX + 32.0f, 20.0f) + hudPosition, Zeni::Point2f(1.0f, 1.0f));
	ur = Zeni::Vertex2f_Texture(Zeni::Point2f(rocketX + 32.0f,  12.0f) + hudPosition, Zeni::Point2f(1.0f, 0.0f));
	material = Zeni::Material("weapon_rocket");
	quad = Zeni::Quadrilateral<Zeni::Vertex2f_Texture>(ul, ll, lr, ur);
	quad.fax_Material(&material);
	vr.render(quad);

	str.str("");
	str << m_raceCar->getRockets().remaining;
	Zeni::get_Fonts()["silkscreen_normal"].render_text(Zeni::String(str.str()), Zeni::Point2f(rocketX + 32, -6) + hudPosition, Zeni::Color(0xFFFFFFFF));

	ul = Zeni::Vertex2f_Texture(Zeni::Point2f(jumpX, 0.0f) + hudPosition, Zeni::Point2f(0.0f, 0.0f));
	ll = Zeni::Vertex2f_Texture(Zeni::Point2f(jumpX, 32.0f) + hudPosition, Zeni::Point2f(0.0f, 1.0f));
	lr = Zeni::Vertex2f_Texture(Zeni::Point2f(jumpX + 32.0f, 32.0f) + hudPosition, Zeni::Point2f(1.0f, 1.0f));
	ur = Zeni::Vertex2f_Texture(Zeni::Point2f(jumpX + 32.0f,  0.0f) + hudPosition, Zeni::Point2f(1.0f, 0.0f));
	material = Zeni::Material("jump");
	quad = Zeni::Quadrilateral<Zeni::Vertex2f_Texture>(ul, ll, lr, ur);
	quad.fax_Material(&material);
	vr.render(quad);

	str.str("");
	str << m_raceCar->getJumps().remaining;
	Zeni::get_Fonts()["silkscreen_normal"].render_text(Zeni::String(str.str()), Zeni::Point2f(jumpX + 32, -6) + hudPosition, Zeni::Color(0xFFFFFFFF));

	ul = Zeni::Vertex2f_Texture(Zeni::Point2f(mineX, 0.0f) + hudPosition, Zeni::Point2f(0.0f, 0.0f));
	ll = Zeni::Vertex2f_Texture(Zeni::Point2f(mineX, 32.0f) + hudPosition, Zeni::Point2f(0.0f, 1.0f));
	lr = Zeni::Vertex2f_Texture(Zeni::Point2f(mineX + 32.0f, 32.0f) + hudPosition, Zeni::Point2f(1.0f, 1.0f));
	ur = Zeni::Vertex2f_Texture(Zeni::Point2f(mineX + 32.0f,  0.0f) + hudPosition, Zeni::Point2f(1.0f, 0.0f));
	material = Zeni::Material("weapon_mine");
	quad = Zeni::Quadrilateral<Zeni::Vertex2f_Texture>(ul, ll, lr, ur);
	quad.fax_Material(&material);
	vr.render(quad);

	str.str("");
	str << m_raceCar->getMines().remaining;
	Zeni::get_Fonts()["silkscreen_normal"].render_text(Zeni::String(str.str()), Zeni::Point2f(mineX + 32, -6) + hudPosition, Zeni::Color(0xFFFFFFFF));

	str.str("");
	str << "Lap " << m_raceCar->getCompletedLaps();
	Zeni::get_Fonts()["silkscreen_normal"].render_text(Zeni::String(str.str()), Zeni::Point2f(0, -6), Zeni::Color(0xFFFFFFFF));
}