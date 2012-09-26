#include <zenilib.h>
#include <sstream>
#include "PlayState.h"
#include "Level.h"
#include "RaceCar.h"
#include "Actor.h"
#include "Input.h"

using namespace Zeni;

PlayState::PlayState()
	  : m_timePassed(0.0f) {
    set_pausable(true);
	m_chronometer.start();
	addBody(new RaceCar());
	//addBody(new RaceCar(Zeni::Point2f(100.0, 100.0)));
	m_viewports.push_back(Viewport(Zeni::Point2f(), Zeni::Vector2f(1.0f, 1.0f)));
	m_level = nullptr;
	loadLevel("levels/level1");
}

PlayState::~PlayState() {
	for (int i=0; i < m_bodies.size(); i++) {
		delete m_bodies[i];
	}
	delete m_level;
}

void PlayState::addBody(Body * body) {
	m_bodies.push_back(body);
}
	
void PlayState::removeBody(Body * body) {
	for (int i=0; i < m_bodies.size(); i++) {
		if (m_bodies[i] == body) {
			delete m_bodies[i];
			m_bodies.erase(m_bodies.begin() + i);
			return;
		}
	}
}

void PlayState::loadLevel(Zeni::String fileName) {
	if (m_level != nullptr) delete m_level;
	m_level = new Level(fileName);

	while (!m_bodies.empty()) {
		removeBody(m_bodies[0]);
	}

	Zeni::String fileData;
	Zeni::File_Ops::load_asset(fileData, fileName + ".entities");
	std::string s = "";
	for (int i=0; i<fileData.size(); i++) {
		if (fileData[i] != '\r') {
			s += fileData[i];
		}
	}
	std::istringstream fileDataStream(s);
	std::string line;
	for (int y=0; std::getline(fileDataStream, line); y++) {
		std::istringstream lineStream(line);
		std::string objectType;
		lineStream >> objectType;
		if (objectType == "RaceCar") {
			float x, y, rotation;
			lineStream >> x >> y >> rotation;
			RaceCar* car = new RaceCar(Zeni::Point2f(x*32.0, y*32.0));
			addBody(car);
			m_trackedBodies.push_back(car);
		}
	}
}

void PlayState::applyStateModifications(StateModifications &stateModifications) {
	for (std::list<Body*>::iterator it = stateModifications.bodyAdditions.begin(); it != stateModifications.bodyAdditions.end(); it++) {
		addBody(*it);
	}
	for (std::list<Body*>::iterator it = stateModifications.bodyRemovals.begin(); it != stateModifications.bodyRemovals.end(); it++) {
		removeBody(*it);
	}
	for (std::list<Tile>::iterator it = stateModifications.tileChanges.begin(); it != stateModifications.tileChanges.end(); it++) {
		m_level->changeTile(*it);
	}
}

const std::vector<std::vector<Body*>> PlayState::getBodyCollisions() {
	std::vector<std::vector<Body*>> collisions(m_bodies.size());
	for (int i = 0; i < m_bodies.size(); i++) {
		if (m_bodies[i]->willDetectCollisionsWithBodies()) {
			for (int j = i + 1; j < m_bodies.size(); j++) {
				if (m_bodies[i]->isTouching(*m_bodies[j])) {
					collisions[i].push_back(m_bodies[j]);
					collisions[j].push_back(m_bodies[i]);
				}
			}
		}
	}
	return collisions;
}

void PlayState::perform_logic() {
    const float timePassed = m_chronometer.seconds();
    const float timeStep = timePassed - m_timePassed;
    m_timePassed = timePassed;

	StateModifications stateModifications = StateModifications();

	for (int i=0; i < m_bodies.size(); i++) {
		m_bodies[i]->stepPhysics(timeStep);
	}
	std::vector<std::vector<Body*>> bodyCollisions = getBodyCollisions();
    for (int i=0; i < m_bodies.size(); i++) {
		std::vector<Tile*> tileCollisions;
		if (m_bodies[i]->willDetectCollisionsWithTiles()) tileCollisions = m_level->getCollidingTiles(*m_bodies[i]);
		m_bodies[i]->handleCollisions(timeStep, tileCollisions, bodyCollisions[i]);
		Actor * actor = dynamic_cast<Actor*>(m_bodies[i]);
		if (actor != nullptr) {
			stateModifications.combine(actor->act(tileCollisions, bodyCollisions[i])); // TODO: pass body collisions or possible simply store these beforehand so that body can acces these too...
		}
	}

	for (std::vector<Viewport>::iterator it = m_viewports.begin(); it != m_viewports.end(); it++) {
		Zeni::Vector2f directionalOffset = Zeni::Vector2f(600.0f/3.0, 500.0f/3.0).multiply_by(m_bodies[0]->getRotationVector());
		it->stepViewportPosition(timeStep, m_bodies[0]->getPosition() - Zeni::Vector2f(600.0f, 500.0f) + directionalOffset);
	}
	

	applyStateModifications(stateModifications);

	Input::stepInput();
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
	for (std::vector<Viewport>::iterator it = m_viewports.begin(); it != m_viewports.end(); it++) {
		/*Zeni::Vector2f directionalOffset = Zeni::Vector2f(600.0f/3.0, viewHeight/3.0).multiply_by(m_bodies[playerNum]->getRotationVector());
		Zeni::Point2f upperLeft = m_bodies[playerNum]->getPosition() - Zeni::Vector2f(600.0f, viewHeight) + directionalOffset;
		Zeni::Point2f lowerRight = m_bodies[playerNum]->getPosition() + Zeni::Vector2f(600.0f, viewHeight) + directionalOffset;*/
			
		it->render(*m_level, m_bodies);
	}
}