#include <zenilib.h>
#include <vector>
#include <sstream>
#include "Level.h"
#include "Tile.h"
#include "Body.h"

enum { GRASS = 0x00FF00FF, BLACK = 0x000000FF, DIRT = 0x7F3300FF, FLOOR_BLUE_OUTLINE = 0x0000FFFF, WALL_RED_OUTLINE = 0xFF0000FF};

Zeni::String getImageNameFromColor(const Uint32 color) {
	switch (color) {
	case GRASS: 
		return "grass";
	case BLACK:
		return "black";
	case DIRT:
		return "dirt";
	case FLOOR_BLUE_OUTLINE:
		return "floor-blue_outline";
	case WALL_RED_OUTLINE:
		return "wall-red_outline";
	default:
		return "placeholder";
	}
}

float getRewardFromColor(const Uint32 color) {
	switch (color) {
	case GRASS: 
		return 0;
	case BLACK:
		return -10;
	case DIRT:
		return 0;
	case FLOOR_BLUE_OUTLINE:
		return 0;
	case WALL_RED_OUTLINE:
		return -10;
	default:
		return -10;
	}
}

// TODO: implement
Level::Level(Zeni::String fileName) {
	m_tileSize = Zeni::Vector2f(32.0, 32.0);
	for (long i=1; Zeni::get_Textures().find(i); i++) {
		m_numTextures = i;
	}

	std::vector<std::vector<float>> rewards;

	Zeni::Image levelImage = Zeni::Image(fileName + "-tiles.png");
	for (int y=0; y < levelImage.height(); y++) {
		std::vector<float> rewardsRow;
		for (int x=0; x < levelImage.width(); x++) {
			Uint32 color = levelImage.extract_RGBA(Zeni::Point2i(x, y));
			Tile newTile(Zeni::Vector2f(x * m_tileSize.i, y * m_tileSize.j), m_tileSize, getImageNameFromColor(color));
			setTile(newTile);

			rewardsRow.push_back(getRewardFromColor(color));
		}
		rewards.push_back(rewardsRow);
	}

	m_navMap = NavigationMap(rewards, m_tileSize);
	for (int i=0; i<20; i++) m_navMap.iterateValues();
}

Level::~Level(){
	while(!m_tiles.empty()){
		std::vector<Tile*> row = m_tiles.back();
		while(!row.empty()) {
			Tile * tile = row.back();
			row.pop_back();
			delete tile;
		}
		m_tiles.pop_back();
	}
}

void Level::setTile(Tile tile) {
	int tileColumn = (int)(tile.getPosition().x / m_tileSize.i);
	int tileRow = (int)(tile.getPosition().y / m_tileSize.j);
	if (tileColumn > m_tiles.size()) {
		return;
	}
	bool mustAddColumn = tileColumn == m_tiles.size();
	if (mustAddColumn && tileRow == 0) {
		Tile * newTile = new Tile(tile);
		std::vector<Tile*> * column = new std::vector<Tile*>();
		column->push_back(newTile);
		m_tiles.push_back(*column);
	} else if (mustAddColumn && tileRow > 0) {
		return;
	} else {
		std::vector<Tile*> column = m_tiles.at(tileColumn);
		if (tileRow > column.size()) {
			return;
		}
		if (tileRow == column.size()) {
			Tile * newTile = new Tile(tile);
			column.push_back(newTile);
		} else {
			*column[tileRow] = tile;
		}
		m_tiles.at(tileColumn) = column;
	}
}

void Level::render(Zeni::Point2f offset, Zeni::Vector2f screenSize) const{
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Vector2f tileScreenEdge((int)((screenSize.x + offset.x)/m_tileSize.i) + 1, (int)((screenSize.y + offset.y)/m_tileSize.j) + 1);
	std::vector<std::vector<Tile*>> tileVectors;
	for (int i=0; i < m_numTextures; i++) {
		std::vector<Tile*> tiles;
		tileVectors.push_back(tiles);
	}
	for (int tileX = offset.x/m_tileSize.i; tileX < tileScreenEdge.i; tileX++) {
		for (int tileY = offset.y/m_tileSize.j; tileY < tileScreenEdge.j; tileY++) {
			if (tileX >= m_tiles.size() || tileY >= m_tiles[tileX].size()) {
				continue;
			}
			Tile * tile = m_tiles[tileX][tileY];
			tileVectors[tile->getImageId() - 1].push_back(tile);
		}
	}
	for (int i = 0; i < m_numTextures; i++) {
		vr.apply_Texture(Zeni::get_Textures()[i + 1]);
		std::vector<Tile*> tiles = tileVectors[i];
		for (int j = 0; j < tiles.size(); j++) {
			Zeni::Point2f pos = tiles[j]->getPosition();
			Zeni::Vertex2f_Texture p1 = Zeni::Vertex2f_Texture(pos, Zeni::Point2f(0.0f, 0.0f));
			Zeni::Vertex2f_Texture p2 = Zeni::Vertex2f_Texture(pos + m_tileSize.get_j(), Zeni::Point2f(0.0f, 1.0f));
			Zeni::Vertex2f_Texture p3 = Zeni::Vertex2f_Texture(pos + m_tileSize, Zeni::Point2f(1.0f, 1.0f));
			Zeni::Vertex2f_Texture p4 = Zeni::Vertex2f_Texture(pos + m_tileSize.get_i(), Zeni::Point2f(1.0f, 0.0f));
			Zeni::Quadrilateral<Zeni::Vertex2f_Texture> tile = Zeni::Quadrilateral<Zeni::Vertex2f_Texture>(p1, p2, p3, p4);
			vr.render(tile);
		}
		vr.unapply_Texture();
	}
	for (int tileX = offset.x/m_tileSize.i; tileX < tileScreenEdge.i; tileX++) {
		for (int tileY = offset.y/m_tileSize.j; tileY < tileScreenEdge.j; tileY++) {
			if (tileX >= m_tiles.size() || tileY >= m_tiles[tileX].size()) {
				continue;
			}
			Tile * tile = m_tiles[tileX][tileY];
			tileVectors[tile->getImageId() - 1].push_back(tile);
			std::ostringstream str;
			str << m_navMap.getSuggestedDirectionAtPosition(Zeni::Point2f(tileX * m_tileSize.i, tileY * m_tileSize.j));
			Zeni::get_Fonts()["tiny"].render_text(Zeni::String(str.str()), Zeni::Point2f(tileX * m_tileSize.i, tileY * m_tileSize.j), Zeni::Color(0xFF0000FF));
		}
	}
}

const std::vector<Tile*> Level::getCollidingTiles(const Body &body) const {
	std::vector<Tile*> tiles;
	// Radius bounding box test
	std::pair<Zeni::Point2f, Zeni::Point2f> boundingBox = body.getBoundingBox();
	for (int tileX = boundingBox.first.x/m_tileSize.i; tileX < boundingBox.second.x/m_tileSize.i; tileX++) {
		for (int tileY = boundingBox.first.y/m_tileSize.j; tileY < boundingBox.second.y/m_tileSize.j; tileY++) {
			if (tileX >= m_tiles.size() || tileY >= m_tiles[tileX].size()) {
				continue;
			}
			Tile * tile = m_tiles[tileX][tileY];
			if (body.isTouching(*tile)) {
				tiles.push_back(tile);
			}
		}
	}
	return tiles;
}

// TODO: Implement
const Tile Level::getTile(Zeni::Point2f position) {
	return Tile();
}

void Level::changeTile(Tile tile) {
	*m_tiles[tile.getPosition().x/m_tileSize.i][tile.getPosition().y/m_tileSize.y] = tile;
}