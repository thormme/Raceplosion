#include <zenilib.h>
#include "RaceResultsState.h"
#include <sstream>

void RaceResultsState::render() {
	Widget_Gamestate::render();

	Zeni::Font &fr = Zeni::get_Fonts()["system_36_800x600"];


	std::ostringstream str;
	str << "Player " << m_finishOrderedPlayers[0] + 1 << " is the winner!";

	fr.render_text(Zeni::String(str.str()),
					Zeni::Point2f(400.0f, 150.0f - 0.5f * fr.get_text_height()),
					Zeni::get_Colors()["title_text"],
					Zeni::ZENI_CENTER);

	Zeni::Vector2f carImageSize(100.0f, 50.0f);
	for (int i=0; i < m_finishOrderedPlayers.size(); i++) {
		Zeni::Point2f carImagePosition(800.0f/(m_finishOrderedPlayers.size())*m_finishOrderedPlayers[i] + 50.0f, 450.0f);

		std::ostringstream place;
		place << i + 1;

		fr.render_text(Zeni::String(place.str()),
			carImagePosition + Zeni::Vector2f(50.0f, -40.0f),
						Zeni::get_Colors()["title_text"],
						Zeni::ZENI_CENTER);

		Zeni::render_image(m_players[m_finishOrderedPlayers[i]]->getCarImage(), carImagePosition, carImagePosition + carImageSize);
	}
}