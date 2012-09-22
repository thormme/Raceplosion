#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include <zenilib.h>
#include "Level.h"
#include "Actor.h"
#include "Viewport.h"

class PlayState : public Zeni::Gamestate_Base {
	PlayState(const PlayState &);
	PlayState operator=(const PlayState &);

public:
	PlayState();
	
	virtual ~PlayState();

private:
	Level m_level;
	std::vector<Body*> m_bodies;
	std::vector<Viewport> m_viewports;
	Zeni::Chronometer<Zeni::Time> m_chronometer;
	float m_timePassed;

	const std::vector<std::vector<Body*>> getBodyCollisions(); ///< Get collisions between bodies, returns vector for each Body

	void addBody(Body * body); ///< Add a body to the world.

	void removeBody(Body * body); ///< Remove a body from the world.

	void applyStateModifications(StateModifications &stateModifications);

	void perform_logic();

	void on_push();

	void on_pop();

	void on_key(const SDL_KeyboardEvent &event);

	void render();

};
#endif