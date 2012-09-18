#include <zenilib.h>
#include "Input.h"

bool Input::keys[] = {0};
bool Input::keysPressed[] = {0};
bool Input::keysReleased[] = {0};

void Input::updateKey(SDLKey key, bool isDown) {
	keys[(int)key] = isDown;
	if (isDown) {
		keysPressed[(int)key] = true;
	} else {
		keysReleased[(int)key] = true;
	}
}

bool Input::isKeyDown(SDLKey key) {
	return keys[(int)key];
}

bool Input::isKeyPressed(SDLKey key) {
	return keysPressed[(int)key];
}

bool Input::isKeyReleased(SDLKey key) {
	return keysReleased[(int)key];
}

void Input::stepInput() {
	memset(keysPressed, 0, 1024 * sizeof keysPressed[0]);
	memset(keysReleased, 0, 1024 * sizeof keysReleased[0]);
}