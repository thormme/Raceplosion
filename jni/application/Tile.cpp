#include <zenilib.h>
#include "Tile.h"

Tile::Tile(const Zeni::Point2f &position,
	const Zeni::String &image): m_image(image), m_position(position){
}

Tile::~Tile(){
}

Zeni::String Tile::getImage() {
	return m_image;
}
Zeni::Point2f Tile::getPosition() {
	return m_position;
}