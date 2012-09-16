#ifndef LEVEL_H
#define LEVEL_H
#include <zenilib.h>
#include <vector>
#include "Tile.h"

class Level {

private:
	std::vector< std::vector<Tile*> > m_tiles;
	std::vector< std::string > m_tileTypes;
	std::multimap< std::string, Tile* > m_tileGroups;
	Zeni::Vector2f m_size;
	Zeni::Vector2f m_tileSize;

	void setTile(Tile tile);

public:
	Level(const Zeni::String fileName = "levels/lvl1.csv");

	virtual ~Level();

	void render(); ///< Render the level tiles

	const Tile getTile(Zeni::Point2f position);
};
#endif