#include <zenilib.h>
#include "Tile.h"

Tile::Tile(const Tile& tile) : GameObject(tile) {
	m_image = Zeni::String(tile.m_image);
	m_type = Zeni::String(tile.m_type); // Name of the image to draw
	m_imageId = tile.m_imageId;

	m_solid = tile.m_solid;
	m_pit = tile.m_pit;
	m_floating = tile.m_floating;
}

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
	m_solid = false;
	m_pit = false;
	m_floating = false;
	if (m_type == "placeholder" || m_type == "wall") {
		m_solid = true;
	}
	if (m_type == "black") {
		m_pit = true;
	}
	if (m_type == "bridge") {
		m_floating = true;
	}
	if (m_type == "bridge_wall") {
		m_solid = true;
		m_floating = true;
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
const bool Tile::isFloating() const {
	return m_floating;
}