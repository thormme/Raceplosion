#include <zenilib.h>
#include "Player.h"
#include "UserData.h"

class RaceResultsState : public Zeni::Widget_Gamestate {
	RaceResultsState(const RaceResultsState &);
	RaceResultsState operator=(const RaceResultsState &);

	std::vector<Player*> m_players;
	std::vector<int> m_finishOrderedPlayers;

	int m_finishPlace;

public:
	RaceResultsState(std::vector<Player*> players, std::vector<int> finishOrderedPlayers, Zeni::String mapName);

private:

	void on_key(const SDL_KeyboardEvent &event) {
		if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED) {
			Zeni::get_Game().pop_state();
		}
	}

	void render();
};