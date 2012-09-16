/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include <sstream>
#include "Level.h"
#include "RaceCar.h"
#include "Actor.h"
#include "Input.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Play_State : public Gamestate_Base {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State()
  : m_playa(), 
    m_timePassed(0.0f),
	m_level() {
    set_pausable(true);
	m_chronometer.start();
  }

private:
  Level m_level;
  RaceCar m_playa;
  Chronometer<Time> m_chronometer;
  float m_timePassed;

  void perform_logic() {
    const float timePassed = m_chronometer.seconds();
    const float timeStep = timePassed - m_timePassed;
    m_timePassed = timePassed;

	m_playa.stepPhysics(timeStep);
    m_playa.act();

	/*std::ostringstream str;
	str << 1.0/timeStep << "\n";
    OutputDebugString( str.str().c_str());*/
  }

  void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    //get_Game().joy_mouse.enabled = false;
  }

  void on_pop() {
    //get_Window().mouse_grab(false);
    get_Window().mouse_hide(false);
    //get_Game().joy_mouse.enabled = true;
  }

  void on_key(const SDL_KeyboardEvent &event) {
	  if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		  Input::updateKey(event.keysym.sym, event.type == SDL_KEYDOWN);
	  }
	  Gamestate_Base::on_key(event);
  }

  void render() {
    get_Video().set_2d();

	m_level.render();
    m_playa.render();
  }

};

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

    Zeni::Font &fr = get_Fonts()["title"];

    fr.render_text(
#if defined(_WINDOWS)
                   "ALT+F4"
#elif defined(_MACOSX)
                   "Apple+Q"
#else
                   "Ctrl+Q"
#endif
                           " to Quit",
                   Point2f(400.0f, 300.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_CENTER);
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("zenilib Application");

      get_Joysticks();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().joy_mouse.enabled = true;

      return new Title_State<Play_State, Instructions_State>("Raceplosion!");
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
