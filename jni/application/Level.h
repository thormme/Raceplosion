#ifndef LEVEL_H
#define LEVEL_H
#include <zenilib.h>
#include <vector>
#include "Tile.h"

class Level {

private:
	std::vector< std::vector<Tile> > m_tiles;
	Zeni::Vector2f size;

public:
	Level(const Zeni::String fileName);

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Level();

	void render();

	Tile getTile(Zeni::Point2f position);
};
#endif