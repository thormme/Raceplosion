#include <string>
#include <map>

class UserData {
	//static std::map<std::string, int> m_bestPosition;
	//std::map<std::string, int> readMapPositions();
public:
	static void setBestPosition(std::string mapName, int position);
	static int getBestPosition(std::string mapName);
};