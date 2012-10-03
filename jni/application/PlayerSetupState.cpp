#include <zenilib.h>
#include "PlayerSetupState.h"
#include "Player.h"
#include "PlayState.h"

std::vector<Player*> players;

PlayerSetupState::PlayerSetupState() : Widget_Gamestate(std::make_pair(Zeni::Point2f(0.0f, 0.0f), Zeni::Point2f(800.0f, 600.0f))) {
	m_widgets.lend_Widget(play_button);
	for (int i=0; i < 4; i++) {
		players.push_back(new Player());
		m_controls.push_back(new ControlTypeSelect(i, Zeni::Point2f(200.0f*(float)(i), 0.0f), Zeni::Point2f(200.0f*(float)(i+1),50.0f), Zeni::Point2f(200.0f*(float)(i), 0.0f), Zeni::Point2f(200.0f*(float)(i+1),300), "system_36_800x600"));
		if (i > 0) {
			m_controls[i]->select_option("Computer");
		}
		m_controls[i]->on_accept(m_controls[i]->get_selected());
		m_widgets.lend_Widget(*m_controls[i]);
	}
}

PlayerSetupState::~PlayerSetupState() {
	for (int i=0; i < m_controls.size(); i++) {
		delete m_controls[i];
	}
	m_controls.clear();
	for (int i=0; i < players.size(); i++) {
		delete players[i];
	}
	players.clear();
}

PlayerSetupState::ControlTypeSelect::ControlTypeSelect(int player, const Zeni::Point2f &upper_left_, const Zeni::Point2f &lower_right_, const Zeni::Point2f &expanded_upper_left_, const Zeni::Point2f &expanded_lower_right_, const Zeni::String &font_)
		: m_player(player), Selector(upper_left_, lower_right_, expanded_upper_left_, expanded_lower_right_, font_) {
			
	add_option("Keyboard");
	add_option("Gamepad 1");
	add_option("Gamepad 2");
	add_option("Gamepad 3");
	add_option("Gamepad 4");
	add_option("Computer");

	select_option("Keyboard");
}

void PlayerSetupState::ControlTypeSelect::on_accept(const Zeni::String &option) {
	if (option == "Keyboard") {
		delete players[m_player];
		players[m_player] = new Player();
		players[m_player]->setControls();
	} else if (option == "Gamepad 1") {
		delete players[m_player];
		players[m_player] = new Player();
		players[m_player]->setControls(0);
	} else if (option == "Gamepad 2") {
		delete players[m_player];
		players[m_player] = new Player();
		players[m_player]->setControls(1);
	} else if (option == "Gamepad 3") {
		delete players[m_player];
		players[m_player] = new Player();
		players[m_player]->setControls(2);
	} else if (option == "Gamepad 4") {
		delete players[m_player];
		players[m_player] = new Player();
		players[m_player]->setControls(3);
	} else/* if (option == "Computer")*/ {
		delete players[m_player];
		players[m_player] = new AIPlayer();
	}

	switch(m_player) {
	case 0:
		players[m_player]->setCarImage("race_car");
		break;
	case 1:
		players[m_player]->setCarImage("race_car-blue");
		break;
	case 2:
		players[m_player]->setCarImage("race_car-silver");
		break;
	case 3:
		players[m_player]->setCarImage("race_car-yellow");
		break;
	}

	Selector::on_accept(option);
}

void PlayerSetupState::Play_Button::on_accept() {
	Zeni::get_Game().push_state(new PlayState(players));
}