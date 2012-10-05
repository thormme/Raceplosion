/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include "PlayState.h"
#include "PlayerSetupState.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["system_26_800x600"];

    fr.render_text("Welcome to raceplosion, a game where you try to cross the\n\
					finish line before your opponents.\n\
					To do this you must employ all your driving skills and gadgets.\n\
					Gadgets include:\n\
					\n\
					Jump Jets:\n\
					These allow you to jump over pits and wall, be careful not to\n\
					jump off the track though!\n\
					\n\
					Missiles:\n\
					Shoot these at your enemies to hurt and destroy them.\n\
					They are also good for destroying walls.\n\
					\n\
					Mines:\n\
					Lay these on the track to damage those behind you.\n\
					\n\
					If you are destroyed you will respawn at the last waypoint in 2 seconds.\n\
					You may respawn at any time by hitting the respawn key.\n\
					Respawning refills all weapon/jump charges.\n\
					\n\
					Escape/Start will alow you to return from any screen.\n\
					Try to get gold on all of the tracks, good luck!",
                   Point2f(10.0f, 25.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_LEFT);
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("Raceplosion");

      get_Joysticks();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().joy_mouse.enabled = true;

      return new Title_State<PlayerSetupState, Instructions_State>("Raceplosion!");
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}
