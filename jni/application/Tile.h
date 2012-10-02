#ifndef TILE_H
#define TILE_H
#include <zenilib.h>
#include "GameObject.h"

class Tile : public GameObject {

private:
	Zeni::String m_image;
	Zeni::String m_type; // Name of the image to draw
	long m_imageId;

	bool m_solid;
	bool m_pit;
	bool m_floating;

public:
	Tile(const Tile& tile);
	Tile(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f),
		const Zeni::String &image = "placeholder");

	const Zeni::String getImage() const;
	const Zeni::String getType() const;
	const long getImageId() const;

	const bool Tile::isSolid() const;
	const bool Tile::isPit() const;
	const bool Tile::isFloating() const;
};
#endif