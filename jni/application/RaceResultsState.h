#include <zenilib.h>
#include "Player.h"

class RaceResultsState : public Zeni::Widget_Gamestate {
	RaceResultsState(const RaceResultsState &);
	RaceResultsState operator=(const RaceResultsState &);

	std::vector<Player*> m_players;
	std::vector<int> m_finishOrderedPlayers;

public:
	RaceResultsState(std::vector<Player*> players, std::vector<int> finishOrderedPlayers)
		: Widget_Gamestate(std::make_pair(Zeni::Point2f(0.0f, 0.0f), Zeni::Point2f(800.0f, 600.0f))),
		m_players(players), m_finishOrderedPlayers(finishOrderedPlayers) {
  }

private:

	void on_key(const SDL_KeyboardEvent &event) {
		if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED) {
			Zeni::get_Game().pop_state();
		}
	}

	void render();
};