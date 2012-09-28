#include <zenilib.h>
#include "Tile.h"

Tile::Tile(const Zeni::Point2f &position,
		const Zeni::Vector2f &size,
		const Zeni::String &image): GameObject(position, size), m_image(image) {
	m_imageId = Zeni::get_Textures().get_id(image);
	size_t pos = image.find_first_of('-');
	if (pos != std::string::npos) {
		m_type = image.substr(0, image.find_first_of('-'));
	} else {
		m_type = image;
	}
	if (m_type == "placeholder" || m_type == "wall") {
		m_solid = true;
	} else {
		m_solid = false;
	}
	if (m_type == "black") {
		m_pit = true;
	} else {
		m_pit = false;
	}
}

const Zeni::String Tile::getImage() const {
	return m_image;
}
const Zeni::String Tile::getType() const {
	return m_type;
}
const long Tile::getImageId() const {
	return m_imageId;
}
const bool Tile::isSolid() const {
	return m_solid;
}
const bool Tile::isPit() const {
	return m_pit;
}