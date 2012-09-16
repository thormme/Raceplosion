#include <zenilib.h>
#include <vector>
#include <sstream>
#include "Level.h"
#include "Tile.h"

// TODO: implement
Level::Level(Zeni::String fileName) {
	m_tileSize = Zeni::Vector2f(32.0, 32.0);
	//std::map< Zeni::String, std::iterator<Tile>
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
		m_tileGroups.insert(std::pair<std::string, Tile*>(newTile->getImage().std_str(), newTile));
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
			m_tileGroups.insert(std::pair<std::string, Tile*>(newTile->getImage().std_str(), newTile));
		} else {
			*column[tileRow] = tile;
		}
		m_tiles.at(tileColumn) = column;
	}

	if (std::find(m_tileTypes.begin(), m_tileTypes.end(), tile.getImage().std_str()) == m_tileTypes.end()) {
		m_tileTypes.push_back(tile.getImage().std_str());
	}
}

void Level::render(){
	Zeni::Video &vr = Zeni::get_Video();
    vr.set_2d(std::make_pair(Zeni::Point2f(0.0f, 0.0f), Zeni::Point2f(800.0f, 600.0f)), true);
	for (int i = 0; i < m_tileTypes.size(); i++) {
		vr.apply_Texture(Zeni::get_Textures()[Zeni::String(m_tileTypes[i])]);
		std::multimap<std::string, Tile*>::iterator it;
		std::pair<std::multimap<std::string, Tile*>::iterator, std::multimap<std::string, Tile*>::iterator> ret;
		ret = m_tileGroups.equal_range(m_tileTypes[i]);
		for (it=ret.first; it!=ret.second; ++it) {
			Zeni::Point2f pos = it->second->getPosition();
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

// TODO: Implement
const Tile Level::getTile(Zeni::Point2f position) {
	return Tile();
}