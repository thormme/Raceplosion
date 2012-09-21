#include <zenilib.h>
#include <vector>
#include <sstream>
#include "Level.h"
#include "Tile.h"
#include "Body.h"

Zeni::String getImageNameFromColor(const Zeni::Color color) {
	if (color == Zeni::Color(1.0, 0.0, 1.0, 0.129411765)) {
		return "grass";
	}
	if (color == Zeni::Color(1.0, 0.0, 0.0, 0.0)) {
		return "black";
	}
	if (color == Zeni::Color(1.0, 0.498039216, 0.2, 0.0)) {
		return "dirt";
	}
	return "placeholder";
}

// TODO: implement
Level::Level(Zeni::String fileName) {
	Zeni::Image levelImage = Zeni::Image("levels/lvl1.png");
	for (long i=1; Zeni::get_Textures().find(i); i++) {
		m_numTextures = i;
	}
	m_tileSize = Zeni::Vector2f(32.0, 32.0);
	/*Zeni::String fileData;
	Zeni::File_Ops::load_asset(fileData, fileName);
	std::string s = "";
	for (int i=0; i<fileData.size(); i++) {
		if (fileData[i] != '\r') {
			s += fileData[i];
		}
	}
	std::istringstream fileDataStream(s);
	std::string line;*/
	for (int y=0; y < levelImage.height(); y++) {
		for (int x=0; x < levelImage.width(); x++) {
			Tile newTile(Zeni::Vector2f(x * m_tileSize.i, y * m_tileSize.j), getImageNameFromColor(levelImage.extract_Color(Zeni::Point2i(x, y))));
			setTile(newTile);
		}
	}
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

void Level::render(Zeni::Point2f offset, Zeni::Vector2f screenSize){
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Chronometer<Zeni::Time> chrono;
	chrono.start();
	chrono.reset();
	std::ostringstream str;
	Zeni::Vector2f tileScreenEdge((int)((screenSize.x + offset.x)/m_tileSize.i) + 1, (int)((screenSize.y + offset.y)/m_tileSize.j) + 1);
	std::vector<std::vector<Tile*>> tileVectors;
	for (int i=0; i < m_numTextures; i++) {
		std::vector<Tile*> tiles;
		tileVectors.push_back(tiles);
	}
	str << chrono.seconds()*1000.0 << " ";
	chrono.reset();
	for (int tileX = offset.x/m_tileSize.i; tileX < tileScreenEdge.i; tileX++) {
		for (int tileY = offset.y/m_tileSize.j; tileY < tileScreenEdge.j; tileY++) {
			if (tileX >= m_tiles.size() || tileY >= m_tiles[tileX].size()) {
				continue;
			}
			Tile * tile = m_tiles[tileX][tileY];
			tileVectors[tile->getImageId() - 1].push_back(tile);
		}
	}
	str << chrono.seconds()*1000.0 << " ";
	chrono.reset();
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
	str << chrono.seconds()*1000.0 << "\n";
    OutputDebugString( str.str().c_str());
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
			if (body.isTouching(tile->getPosition(), m_tileSize)) {
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