#include <zenilib.h>
#include <sstream>
#include "PlayState.h"
#include "Level.h"
#include "RaceCar.h"
#include "AIPlayer.h"
#include "Player.h"
#include "Waypoint.h"
#include "Actor.h"
#include "Input.h"
#include "Utils.h"
#include "Sensor.h"
#include "Time.h"
#include "RaceCarViewport.h"
#include "RaceResultsState.h"

PlayState::PlayState(std::vector<Player*> players, const Zeni::String &level)
	  : m_timePassed(0.0f),
		m_racers(players) {
    set_pausable(true);
	m_chronometer.start();
	m_level = nullptr;
	loadLevel(level);
	m_numRequiredLaps = 3;
}

PlayState::~PlayState() {
	for (int i=0; i < m_bodies.size(); i++) {
		delete m_bodies[i];
	}
	for (int i=0; i < m_viewports.size(); i++) {
		delete m_viewports[i];
	}
	delete m_level;
}

void PlayState::addBody(Body * body) {
	m_bodies.push_back(body);
}
	
void PlayState::removeBody(Body * body) {
	Body* removalBody = nullptr;
	for (std::vector<Body*>::iterator it = m_bodies.begin(); it != m_bodies.end(); it++) {
		if (*it == body) {
			Body* removalBody = *it;
			m_bodies.erase(it);
			break;
		}
	}
	if (removalBody != nullptr) {
		delete removalBody;
	}
}

void PlayState::loadLevel(Zeni::String fileName) {
	if (m_level != nullptr) delete m_level;
	Zeni::String filePath = "levels/" + fileName;
	m_level = new Level(filePath);
	m_levelName = fileName;

	while (!m_bodies.empty()) {
		removeBody(m_bodies[0]);
	}

	Zeni::String fileData;
	Zeni::File_Ops::load_asset(fileData, filePath + ".entities");
	std::string s = "";
	for (int i=0; i<fileData.size(); i++) {
		if (fileData[i] != '\r') {
			s += fileData[i];
		}
	}
	std::istringstream fileDataStream(s);
	std::string line;
	int unusedPlayer = 0;
	std::vector<RaceCar*> humanRaceCars;
	for (int y=0; std::getline(fileDataStream, line); y++) {
		std::istringstream lineStream(line);
		std::string objectType;
		lineStream >> objectType;
		if (objectType == "RaceCar") {
			float x, y, rotation;
			lineStream >> x >> y >> rotation;
			Player * player = m_racers[unusedPlayer++];
			RaceCar* car = player->getNewCar(Zeni::Point2f(x*32.0, y*32.0), rotation/180.0f*Utils::PI);
			addBody(car);
			AIPlayer * aiPlayer = dynamic_cast<AIPlayer*>(player);
			if (aiPlayer != nullptr) {
				addBody(new Sensor(aiPlayer, Zeni::Vector2f(800.0f, 800.0f)));
			} else {
				humanRaceCars.push_back(car);
				m_trackedBodies.push_back(car);
			}
		} else if (objectType == "Waypoint") {
			float x, y, rotation, width;
			lineStream >> x >> y >> rotation >> width;
			Waypoint* waypoint = new Waypoint(Zeni::Point2f(x*32.0, y*32.0), Zeni::Vector2f(32.0f*width, 32.0f), rotation/180.0f*Utils::PI);
			waypoint->setPosition(waypoint->getPosition());
			addBody(waypoint);
			m_waypoints.push_back(waypoint);
		}
	}
	Zeni::Vector2f screenSize = Zeni::Vector2f(Zeni::Point2f(Zeni::get_Video().get_render_target_size()));
	for (int i=0; i < humanRaceCars.size(); i++) {
		if (humanRaceCars.size() == 1) {
			m_viewports.push_back(new RaceCarViewport(humanRaceCars[i], Zeni::Point2f(), Zeni::Vector2f(1.0f, 1.0f), Zeni::Point2f(), Zeni::Point2f(screenSize.i/screenSize.j*1000.0f, 1000.0f)));
		} else if (humanRaceCars.size() == 2) {
			m_viewports.push_back(new RaceCarViewport(humanRaceCars[i], Zeni::Point2f((double)(i)/2.0f, 0), Zeni::Vector2f(0.5f, 1.0f), Zeni::Point2f(screenSize.i/2.0f, 0.0f), Zeni::Point2f(screenSize.i/screenSize.j*1000.0f/2.0f, 1000.0f)));
		} else if (humanRaceCars.size() > 2) {
			m_viewports.push_back(new RaceCarViewport(humanRaceCars[i], Zeni::Point2f((double)(i%2)/2.0f, (double)(i/2)/2.0f), Zeni::Vector2f(0.5f, 0.5f), Zeni::Point2f(screenSize.i, 0.0f), Zeni::Point2f(screenSize.i/screenSize.j*1000.0f, 1000.0f)));
		}
	}
	std::vector<Zeni::Point2f> goals;
	for (int i=0; i < m_waypoints.size(); i++) {
		goals.push_back(m_waypoints[i]->getCenter());
	}
	m_level->updateNavigationMaps(goals);

	for (int i=0; i < m_racers.size(); i++) {
		AIPlayer * aiPlayer = dynamic_cast<AIPlayer*>(m_racers[i]);
		if (aiPlayer != nullptr) {
			aiPlayer->setNavigationMaps(m_level->getNavigationMaps());
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
	// TODO: not checking that second body is detecting collisions, bug fixes issue where players can jump over waypoints.
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
	const float timeStep = std::min(timePassed - m_timePassed, 50.0f/1000.0f); // Set lower bound on simulation at 20 fps
    m_timePassed = timePassed;

	StateModifications stateModifications = StateModifications();

	// Run physics
	for (int i=0; i < m_bodies.size(); i++) {
		m_bodies[i]->stepPhysics(timeStep);
	}

	// Handle collisions and have actors act
	std::vector<std::vector<Body*>> bodyCollisions = getBodyCollisions();
    for (int i=0; i < m_bodies.size(); i++) {
		std::vector<Tile*> tileCollisions;
		if (m_bodies[i]->willDetectCollisionsWithTiles()) tileCollisions = m_level->getCollidingTiles(*m_bodies[i]);
		m_bodies[i]->handleCollisions(timeStep, tileCollisions, bodyCollisions[i]);
		Actor * actor = dynamic_cast<Actor*>(m_bodies[i]);
		if (actor != nullptr) {
			stateModifications.combine(actor->act(tileCollisions, bodyCollisions[i])); // TODO: possibly simply store body collisions for these beforehand so that body can access these too...
		}
	}

	// Adjust viewports
	for (int i = 0; i < m_viewports.size(); i++) {
		Zeni::Vector2f halfScreen = m_viewports[i]->getWorldViewSize()/2.0f;
		Zeni::Vector2f directionalOffset = (halfScreen/3.0f).multiply_by(m_trackedBodies[i]->getRotationVector());
		m_viewports[i]->stepViewportPosition(timeStep, m_trackedBodies[i]->getPosition() - halfScreen + directionalOffset);
	}

	// Track race progress
	for (int i = 0; i < m_racers.size(); i++) {
		if (m_racers[i]->getLastCar()->getPassedWaypoints().size() == m_waypoints.size() && m_racers[i]->getLastCar()->isTouching(*m_waypoints[0])) {
			m_racers[i]->getLastCar()->setLapCompleted();
			if (m_racers[i]->getLastCar()->getCompletedLaps() == m_numRequiredLaps) {
				m_racers[i]->getLastCar()->setDisabled(true);
				m_finishedRacers.push_back(i);
			}
		}
	}
	if (m_finishedRacers.size() >= m_racers.size() - 1) {
		for (int i = 0; i < m_racers.size(); i++) {
			if (m_racers[i]->getLastCar()->getCompletedLaps() < m_numRequiredLaps) {
				m_finishedRacers.push_back(i);
			}
		}
		Zeni::get_Game().pop_state();
		Zeni::get_Game().push_state(new RaceResultsState(m_racers, m_finishedRacers, m_levelName));
	}
	
	// Apply any body/tile changes made by actors
	applyStateModifications(stateModifications);

	Input::stepInput();
	Time::updateGameTime(timeStep);
	static int counter = 0;
	counter++;
	// TODO: investigate dealing with path changes
	//if(counter%30 == 0) m_level->improveNavigationMaps(counter%m_level->getNavigationMaps().size());
}

void PlayState::on_push() {
    //get_Window().mouse_grab(true);
    Zeni::get_Window().mouse_hide(true);
    Zeni::get_Game().joy_mouse.enabled = false;
}

void PlayState::on_pop() {
    //get_Window().mouse_grab(false);
    Zeni::get_Window().mouse_hide(false);
    Zeni::get_Game().joy_mouse.enabled = true;
}

void PlayState::on_key(const SDL_KeyboardEvent &event) {
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		Input::updateKey(event.keysym.sym, event.type == SDL_KEYDOWN);
	}
	Gamestate_Base::on_key(event);
}

void PlayState::on_joy_button(const SDL_JoyButtonEvent &event) {
	if (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP) {
		Input::updateJoyButton(event.which, event.button, event.type == SDL_JOYBUTTONDOWN);
	}
	Gamestate_Base::on_joy_button(event);
}

void PlayState::on_joy_axis(const SDL_JoyAxisEvent &event) {
	Input::updateJoyAxisValue(event.which, event.axis, event.value);
	Gamestate_Base::on_joy_axis(event);
}

void PlayState::render() {
	for (std::vector<Viewport*>::iterator it = m_viewports.begin(); it != m_viewports.end(); it++) {
		/*Zeni::Vector2f directionalOffset = Zeni::Vector2f(600.0f/3.0, viewHeight/3.0).multiply_by(m_bodies[playerNum]->getRotationVector());
		Zeni::Point2f upperLeft = m_bodies[playerNum]->getPosition() - Zeni::Vector2f(600.0f, viewHeight) + directionalOffset;
		Zeni::Point2f lowerRight = m_bodies[playerNum]->getPosition() + Zeni::Vector2f(600.0f, viewHeight) + directionalOffset;*/
			
		(*it)->render(*m_level, m_bodies);
	}
}