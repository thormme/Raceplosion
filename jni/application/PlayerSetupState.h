#include <zenilib.h>
#include "Player.h"
#include "AIPlayer.h"
#include "Utils.h"

class PlayerSetupState : public Zeni::Widget_Gamestate {
  PlayerSetupState(const PlayerSetupState &);
  PlayerSetupState operator=(const PlayerSetupState &);

public:
	class ControlTypeSelect : public Zeni::Selector {
	//public:
		ControlTypeSelect(const ControlTypeSelect &);
		ControlTypeSelect & operator=(const ControlTypeSelect &);

		int m_player;

	public:
		ControlTypeSelect(int player, const Zeni::Point2f &upper_left_, const Zeni::Point2f &lower_right_, const Zeni::Point2f &expanded_upper_left_, const Zeni::Point2f &expanded_lower_right_, const Zeni::String &font_);

		void on_accept(const Zeni::String &option);
	};	

	class Play_Button : public Zeni::Text_Button {
		Play_Button(const Play_Button &);
		Play_Button & operator=(const Play_Button &);

		public:
		Play_Button()
			: Text_Button(Zeni::Point2f(200.0f, 250.0f), Zeni::Point2f(600.0f, 310.0f),
				"system_36_800x600", "Play") {
		}

		void on_accept();
	} play_button;

	PlayerSetupState();

	~PlayerSetupState();

private:
	std::vector<ControlTypeSelect*> m_controls;

  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      Zeni::get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();
  }
};