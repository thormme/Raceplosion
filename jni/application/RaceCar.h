#ifndef RACECAR_H
#define RACECAR_H
#include <zenilib.h>
#include "Actor.h"
#include "Waypoint.h"
#include "Rocket.h"
#include "Mine.h"

class Player;
class AIPlayer;

class RaceCar : public Actor {
	friend class Player;
	friend class AIPlayer;
public:
	struct Consumable {
		int max;
		int remaining;
		Consumable(int max = 0, int remaining = -1) : max(max), remaining(remaining) {
			if (remaining == -1) {
				remaining = max;
			}
		}
		void refill() {
			remaining = max;
		}
		int consume(int amount = 1) {
			amount = std::min(amount, remaining);
			remaining -= amount;
			return amount;
		}
	};

private:
	Player *m_driver;
	double m_wheelRotation;
	double m_wheelSeparation;
	double m_traction;
	double m_tireFriction;
	double m_rollingFriction;
	double m_engineForce;
	double m_braking;
	double m_maximumWheelRotation;
	std::vector<Waypoint*> m_passedWaypoints;
	int m_completedLaps;

	double m_timer;
	double m_respawnTimer;
	double m_jumpTimer;
	double m_jumpTimeLimit;
	Zeni::String m_originalImage;
	bool m_respawning;
	bool m_jumping;

	double m_tireSquealTimer;
	
	Consumable m_health;
	Consumable m_rockets;
	Consumable m_mines;
	Consumable m_jumps;

	const Zeni::Vector2f RaceCar::getDirectionalFrictionForce(const double &direction, const double &friction);
	const Zeni::Vector2f getDirectionalVelocity(const double &direction);
	void completeRespawn();
	void completeJump();

protected:
	virtual const StateModifications run(const std::vector<Tile*> &tileCollisions, const std::vector<Body*> &bodyCollisions);
	void setBraking(bool brake);
	void accelerate(double fraction); ///< Accelerate car, positive for forward
	void setWheelRotation(double rotation); ///< Set relative facing direction of wheels
	Rocket* fireRocket(); ///< Returns a new rocket or nullptr if there are none left
	Mine* layMine(); ///< Returns a new min or nullptr if there are none left
	void respawn(); ///< Respawn at the most recently passed waypoint
	void jump(); ///< Disables collision for some time

	const bool isMovingInDirection(const double &direction);

public:
	RaceCar(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		const double rotation = 0.0f,
		const Zeni::String image = "race_car");

	virtual void render();

	virtual void stepPhysics(const double timeStep);
	virtual void handleCollisions(const double timeStep, std::vector<Tile*>, std::vector<Body*>); ///< Run physics necessary to deal with collisions.

	const int getCompletedLaps() const;
	const std::vector<Waypoint*> getPassedWaypoints() const;

	void setLapCompleted();

	const Consumable getHealth() const;
	const Consumable getRockets() const;
	const Consumable getMines() const;
	const Consumable getJumps() const;
};
#endif