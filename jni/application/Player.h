#ifndef PLAYER_H
#define PLAYER_H
#include <zenilib.h>
#include "RaceCar.h"
#include "Tile.h"
#include "Body.h"
#include "Input.h"

class RaceCar;

class Player {
	
public:
	Player();

	RaceCar * getNewCar(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f), const double rotation = 0.0f);

	RaceCar * getLastCar();

	void setControls(const std::vector<InputAction> &controls);
	void setControls(int joyIndex = -1); ///< Set to default control scheme

	void setCarImage(const Zeni::String &image);
	const Zeni::String getCarImage() const;

	virtual const StateModifications driveRaceCar(RaceCar &raceCar, const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);

	virtual void senseSurroundings(std::vector<Tile*> tiles, std::vector<Body*> bodies) {}

private:
	enum Controls {FORWARD = 0, BACKWARD, LEFT, RIGHT, FIRE_ROCKET, LAY_MINE, JUMP, RESPAWN, FINAL_ELEMENT};
	// Player attributes
	std::vector<InputAction> m_controls;

	RaceCar* m_raceCar;
	// Attributes to pass to car
	Zeni::String m_carImage;
protected:

};
#endif