#include <zenilib.h>
#include <fstream>
#include "UserData.h"
#include "Utils.h"
	
void UserData::setBestPosition(std::string mapName, int position) {
	int previousPosition = getBestPosition(mapName);
	if (previousPosition >= 0 && position > previousPosition) {
		return;
	}
	std::map<std::string, int> positions;
	positions.insert(std::make_pair(mapName, position));

	std::ifstream positionFileInput;
	positionFileInput.open(Zeni::get_File_Ops().get_appdata_path().std_str() + "raceplosion.data");
	while (positionFileInput && !positionFileInput.eof()) {
		std::string levelName;
		int tempPos;
		positionFileInput >> levelName >> tempPos;
		positions.insert(std::make_pair(levelName, tempPos));
	}
	positionFileInput.close();

	std::ofstream positionFile(Zeni::get_File_Ops().get_appdata_path().std_str() + "raceplosion.data");
	std::map<std::string, int>::iterator it = positions.begin();
	while (it != positions.end()) {
		positionFile << it->first << " " << it->second;
		it++;
		if (it != positions.end()) {
			positionFile << std::endl;
		}
	}
	positionFile.close();
}

int UserData::getBestPosition(std::string mapName) {
	std::ifstream positionFile;
	positionFile.open(Zeni::get_File_Ops().get_appdata_path().std_str() + "raceplosion.data");
	int position = -1;
	while (positionFile && !positionFile.eof()) {
		std::string levelName;
		int tempPos;
		positionFile >> levelName >> tempPos;
		if (levelName == mapName) {
			position = tempPos;
		}
	}
	positionFile.close();
	return position;
}