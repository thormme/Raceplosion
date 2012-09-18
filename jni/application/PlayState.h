#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include <zenilib.h>
#include "Level.h"
#include "Actor.h"

class PlayState : public Zeni::Gamestate_Base {
	PlayState(const PlayState &);
	PlayState operator=(const PlayState &);

public:
	PlayState();
	
	virtual ~PlayState();

	void addBody(Body * body); ///< Add a body to the world; deletion will be handled.

	void removeBody(Body * body); ///< Remove a body from the world.

private:
	Level m_level;
	std::vector<Body*> m_bodies;
	Zeni::Chronometer<Zeni::Time> m_chronometer;
	float m_timePassed;

	
	std::vector<Body*> m_bodiesToAdd;
	std::vector<Body*> m_bodiesToRemove;

	void applyAddBody(Body * body);
	void applyRemoveBody(Body * body);
	void applyBodyChanges();

	const std::vector<std::vector<Body*>> getBodyCollisions();


	void perform_logic();

	void on_push();

	void on_pop();

	void on_key(const SDL_KeyboardEvent &event);

	void render();

};
#endif