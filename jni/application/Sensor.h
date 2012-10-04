#include <zenilib.h>
#include "Player.h"
#include "Body.h"
#include "Tile.h"

class Sensor : public Body {

private:
	Player * m_owner;

protected:

public:
	Sensor(Player * owner, const Zeni::Vector2f &size);

	virtual void handleCollisions(const double timeStep, std::vector<Tile*> tiles, std::vector<Body*> bodies);

	void render();
};