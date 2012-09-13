#include <zenilib.h>
#include "Input.h"

bool Input::keys[] = {0};

void Input::updateKey(SDLKey key, bool isDown) {
	keys[(int)key] = isDown;
}

bool Input::isKeyDown(SDLKey key) {
	return keys[(int)key];
}