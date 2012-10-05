#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include <zenilib.h>
#include "Level.h"
#include "Actor.h"
#include "Player.h"
#include "AIRaceCar.h"
#include "Viewport.h"
#include "Waypoint.h"

class PlayState : public Zeni::Gamestate_Base {
	PlayState(const PlayState &);
	PlayState operator=(const PlayState &);

public:
	PlayState(std::vector<Player*> players, const Zeni::String &level = "level1");
	
	virtual ~PlayState();

private:
	Level* m_level;
	Zeni::String m_levelName;
	std::vector<Body*> m_bodies;
	std::vector<Waypoint*> m_waypoints;
	std::vector<Body*> m_trackedBodies;
	std::vector<Player*> m_racers;
	std::vector<Viewport*> m_viewports;
	Zeni::Chronometer<Zeni::Time> m_chronometer;
	float m_timePassed;

	int m_numRequiredLaps;
	std::vector<int> m_finishedRacers;

	const std::vector<std::vector<Body*>> getBodyCollisions(); ///< Get collisions between bodies, returns vector for each Body

	void addBody(Body * body); ///< Add a body to the world.

	void removeBody(Body * body); ///< Remove a body from the world.

	void loadLevel(Zeni::String fileName);

	void applyStateModifications(StateModifications &stateModifications);

	void perform_logic();

	void on_push();

	void on_pop();

	void on_key(const SDL_KeyboardEvent &event);

	void on_joy_button(const SDL_JoyButtonEvent &event);

	void on_joy_axis(const SDL_JoyAxisEvent &event);

	void render();

};
#endif