#ifndef INPUT_H
#define INPUT_H
#include <zenilib.h>

class Input {

private:
	// Stores the states of keys
	static bool keys[1024];

public:
	Input();

	// Update the state of a key.
	static void updateKey(SDLKey key, bool isDown);

	// Whether a key is down.
	static bool isKeyDown(SDLKey key);

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Input();
};
#endif