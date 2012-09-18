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

	Input::stepInput();

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
	int numPlayers = 1;
	for (int playerNum = 0; playerNum < numPlayers; playerNum++) {
		if (m_bodies.size() > playerNum) {
			float viewHeight = 500.0f/numPlayers;
			vr.set_2d_view(std::make_pair(m_bodies[playerNum]->getPosition() - Zeni::Point2f(600.0f, viewHeight), m_bodies[playerNum]->getPosition() + Zeni::Point2f(600.0f, viewHeight)),
				std::make_pair(Zeni::Point2i(0.0f, (vr.get_render_target_size().y/numPlayers)*(playerNum)), Zeni::Point2i(vr.get_render_target_size().x, (vr.get_render_target_size().y/numPlayers)*(playerNum+1))), true);
		}

		if (m_bodies.size() > playerNum) m_level.render(m_bodies[playerNum]->getPosition() - screenHalfResolution, Zeni::Vector2f(screenHalfResolution)*2);

		for (int i=0; i < m_bodies.size(); i++) {
			m_bodies[i]->render();
		}
	}
}