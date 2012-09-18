#ifndef INPUT_H
#define INPUT_H
#include <zenilib.h>

class Input {

private:
	// Stores the states of keys
	static bool keys[1024];
	static bool keysPressed[1024];
	static bool keysReleased[1024];

public:
	Input();

	///< Update the state of a key.
	static void updateKey(SDLKey key, bool isDown);

	///< Whether a key is down.
	static bool isKeyDown(SDLKey key);

	///< Whether a key was pressed.
	static bool isKeyPressed(SDLKey key);

	///< Whether a key was released.
	static bool isKeyReleased(SDLKey key);
	
	///< Step input to reset temporary input state.
	static void stepInput();

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Input();
};
#endif