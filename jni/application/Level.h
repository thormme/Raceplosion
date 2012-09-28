#ifndef LEVEL_H
#define LEVEL_H
#include <zenilib.h>
#include <vector>
#include "Tile.h"
#include "Body.h"
#include "NavigationMap.h"

class Level {

private:
	std::vector< std::vector<Tile*> > m_tiles;
	long m_numTextures;
	Zeni::Vector2f m_size;
	Zeni::Vector2f m_tileSize;
	NavigationMap m_navMap;

	void setTile(Tile tile);

public:
	Level(const Zeni::String fileName = "levels/level1");

	virtual ~Level();

	void render(Zeni::Point2f offset, Zeni::Vector2f screenSize) const; ///< Render the level tiles

	const std::vector<Tile*> getCollidingTiles(const Body &body) const;

	const Tile getTile(Zeni::Point2f position);

	void changeTile(Tile tile);
};
#endif