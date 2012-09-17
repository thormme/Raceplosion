#include <zenilib.h>
#include "Tile.h"

Tile::Tile(const Zeni::Point2f &position,
	const Zeni::String &image): m_image(image), m_position(position){
}

Tile::~Tile(){
}

const Zeni::String Tile::getImage() const {
	return m_image;
}
const Zeni::Point2f Tile::getPosition() const {
	return m_position;
}