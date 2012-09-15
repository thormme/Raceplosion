#ifndef TILE_H
#define TILE_H
#include <zenilib.h>

class Tile {

private:
	Zeni::Point2f m_position; // Tile position in level coordinates
	Zeni::String m_image; // Name of the image to draw

public:
	Tile(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const Zeni::String &image = "placeholder");

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Tile();

	Zeni::String getImage();
	Zeni::Point2f getPosition();
};
#endif