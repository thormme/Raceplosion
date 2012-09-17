#include <zenilib.h>
#include <vector>
#include <sstream>
#include "Level.h"
#include "Tile.h"
#include "Body.h"

// TODO: implement
Level::Level(Zeni::String fileName) {
	m_tileSize = Zeni::Vector2f(32.0, 32.0);
	Zeni::String fileData;
	Zeni::File_Ops::load_asset(fileData, fileName);
	std::string s = "";
	for (int i=0; i<fileData.size(); i++) {
		if (fileData[i] != '\r') {
			s += fileData[i];
		}
	}
	std::istringstream fileDataStream(s);
	std::string line;
	for (int y=0; std::getline(fileDataStream, line); y++) {
		std::istringstream lineStream(line);
		std::string tile;
		for (int x=0; std::getline(lineStream, tile, ','); x++) {
			Tile newTile(Zeni::Vector2f(x * m_tileSize.i, y * m_tileSize.j), Zeni::String(tile));
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

	if (std::find(m_tileTypes.begin(), m_tileTypes.end(), tile.getImage().std_str()) == m_tileTypes.end()) {
		m_tileTypes.push_back(tile.getImage().std_str());
	}
}

void Level::render(Zeni::Point2f offset, Zeni::Vector2f screenSize){
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Chronometer<Zeni::Time> chrono;
	chrono.start();
	chrono.reset();
	Zeni::Vector2f tileScreenEdge((int)((screenSize.x + offset.x)/m_tileSize.i) + 1, (int)((screenSize.y + offset.y)/m_tileSize.j) + 1);
	std::vector<std::vector<Tile*>> tileVectors;
	std::map< std::string, int > tileGroups;
	for (int i=0; i < m_tileTypes.size(); i++) {
		tileGroups.insert(std::pair<std::string, int>(m_tileTypes[i].c_str(), i));
		std::vector<Tile*> tiles;
		tileVectors.push_back(tiles);
	}

	for (int tileX = offset.x/m_tileSize.i; tileX < tileScreenEdge.i; tileX++) {
		for (int tileY = offset.y/m_tileSize.j; tileY < tileScreenEdge.j; tileY++) {
			if (tileX >= m_tiles.size() || tileY >= m_tiles[tileX].size()) {
				continue;
			}
			Tile * tile = m_tiles[tileX][tileY];
			int vecInd = tileGroups.find(tile->getImage().std_str())->second;
			tileVectors[vecInd].push_back(tile);
		}
	}
	for (int i = 0; i < tileVectors.size(); i++) {
		vr.apply_Texture(Zeni::get_Textures()[Zeni::String(m_tileTypes[i])]);
		std::vector<Tile*> tiles = tileVectors[i];
		for (int i=0; i < tiles.size(); i++) {
			Zeni::Point2f pos = tiles[i]->getPosition();
			Zeni::Vertex2f_Texture p1 = Zeni::Vertex2f_Texture(pos, Zeni::Point2f(0.0f, 0.0f));
			Zeni::Vertex2f_Texture p2 = Zeni::Vertex2f_Texture(pos + m_tileSize.get_j(), Zeni::Point2f(0.0f, 1.0f));
			Zeni::Vertex2f_Texture p3 = Zeni::Vertex2f_Texture(pos + m_tileSize, Zeni::Point2f(1.0f, 1.0f));
			Zeni::Vertex2f_Texture p4 = Zeni::Vertex2f_Texture(pos + m_tileSize.get_i(), Zeni::Point2f(1.0f, 0.0f));
			Zeni::Quadrilateral<Zeni::Vertex2f_Texture> tile = Zeni::Quadrilateral<Zeni::Vertex2f_Texture>(p1, p2, p3, p4);
			vr.render(tile);
		}
		vr.unapply_Texture();
	}
}

const std::vector<Tile*> Level::getCollidingTiles(const Body &body) const {
	std::vector<Tile*> tiles;
	// Radius bounding box test
	Zeni::Point2f center = body.getSize()/2.0 + body.getPosition();
	double boundingRadius = (body.getSize()/2.0).magnitude();
	for (int tileX = center.x/m_tileSize.i; tileX < (center.x + boundingRadius)/m_tileSize.i; tileX++) {
		for (int tileY = center.y/m_tileSize.j; tileY < (center.y + boundingRadius)/m_tileSize.j; tileY++) {
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