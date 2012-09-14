#ifndef LEVEL_H
#define LEVEL_H
#include <zenilib.h>
#include "Tile.h"

class Level {

private:
	stl::vector<stl::vector<Tile*>> m_tiles;

public:
	Level(Zeni::String fileName);

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Level();

	void render();

	Tile getTile(Zeni::Point2f position);
};
#endif