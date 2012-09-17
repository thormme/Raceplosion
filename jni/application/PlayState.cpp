#include <zenilib.h>
#include <sstream>
#include "PlayState.h"
#include "Level.h"
#include "RaceCar.h"
#include "Actor.h"
#include "Input.h"

using namespace Zeni;

PlayState::PlayState()
	  : m_timePassed(0.0f),
		m_level() {
    set_pausable(true);
	m_chronometer.start();
	addBody(new RaceCar());
	addBody(new RaceCar(Zeni::Point2f(100.0, 100.0)));
}

PlayState::~PlayState() {
	for (int i=0; i < m_bodies.size(); i++) {
		delete m_bodies[i];
	}
}

void PlayState::addBody(Body * body) {
	m_bodiesToAdd.push_back(body);
}

void PlayState::removeBody(Body * body) {
	m_bodiesToRemove.push_back(body);
}

void PlayState::applyAddBody(Body * body) {
	m_bodies.push_back(body);
}
	
void PlayState::applyRemoveBody(Body * body) {
	for (int i=0; i < m_bodies.size(); i++) {
		if (m_bodies[i] == body) {
			delete m_bodies[i];
			m_bodies.erase(m_bodies.begin() + i);
		}
	}
}

void PlayState::applyBodyChanges() {
	for (int i=0; i < m_bodiesToAdd.size(); i++) {
		applyAddBody(m_bodiesToAdd[i]);
	}
	for (int i=0; i < m_bodiesToRemove.size(); i++) {
		applyRemoveBody(m_bodiesToRemove[i]);
	}
	m_bodiesToAdd.clear();
	m_bodiesToRemove.clear();
}

void PlayState::perform_logic() {
    const float timePassed = m_chronometer.seconds();
    const float timeStep = timePassed - m_timePassed;
    m_timePassed = timePassed;

	applyBodyChanges();

	for (int i=0; i < m_bodies.size(); i++) {
		m_bodies[i]->stepPhysics(timeStep);
	}
    for (int i=0; i < m_bodies.size(); i++) {
		Actor * actor = dynamic_cast<Actor*>(m_bodies[i]);
		if (actor != nullptr) {
			actor->act(m_level.getCollidingTiles(*actor), std::vector<Body*>()); // TODO: pass body collisions or possible simply store these beforehand so that body can acces these too...
		}
	}

	std::ostringstream str;
	str << m_bodies.size() << "\n";
    OutputDebugString( str.str().c_str());
}

void PlayState::on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    //get_Game().joy_mouse.enabled = false;
}

void PlayState::on_pop() {
    //get_Window().mouse_grab(false);
    get_Window().mouse_hide(false);
    //get_Game().joy_mouse.enabled = true;
}

void PlayState::on_key(const SDL_KeyboardEvent &event) {
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		Input::updateKey(event.keysym.sym, event.type == SDL_KEYDOWN);
	}
	Gamestate_Base::on_key(event);
}

void PlayState::render() {
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Point2f screenHalfResolution = Zeni::Point2f(600.0f, 500.0f);
	vr.set_2d(std::make_pair(m_bodies[0]->getPosition() - screenHalfResolution, m_bodies[0]->getPosition() + screenHalfResolution), true);

	m_level.render(m_bodies[0]->getPosition() - screenHalfResolution, Zeni::Vector2f(screenHalfResolution)*2);

    for (int i=0; i < m_bodies.size(); i++) {
		m_bodies[i]->render();
	}

	/*Zeni::Texture * tex = vr.create_Texture(Zeni::Point2i(512, 512), 0);
	Zeni::Vertex2f_Texture p1 = Zeni::Vertex2f_Texture(Point2f(0.0f, 0.0f), Zeni::Point2f(0.0f, 0.0f));
	Zeni::Vertex2f_Texture p2 = Zeni::Vertex2f_Texture(Point2f(0.0f, 600.0f), Zeni::Point2f(0.0f, 1.0f));
	Zeni::Vertex2f_Texture p3 = Zeni::Vertex2f_Texture(Point2f(400.0f, 600.0f), Zeni::Point2f(1.0f, 1.0f));
	Zeni::Vertex2f_Texture p4 = Zeni::Vertex2f_Texture(Point2f(400.0f, 0.0f), Zeni::Point2f(1.0f, 0.0f));
	Zeni::Quadrilateral<Zeni::Vertex2f_Texture> tile = Zeni::Quadrilateral<Zeni::Vertex2f_Texture>(p1, p2, p3, p4);
	tile.render_to(tex->apply_Texture();
	vr.render(tile);
	vr.unapply_Texture();*/
}