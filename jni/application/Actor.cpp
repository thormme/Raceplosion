#include <zenilib.h>
#include "Body.h"

class Actor : Body {

private:
	Zeni::Point2f m_position; // Upper left corner
	Zeni::Vector2f m_size; // (width, height)
	bool active;
	
	// Causes the actor to take an action.
	void run() {
		
	}

public:
	Actor(const Zeni::Point2f &position_,
			const Zeni::Vector2f &size_)
			: m_position(position_),
			m_size(size_) {
		active = true;
	}

	void act() {
		if (active) {
			run();
		}
		active = false;
	}

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Actor() {}
};