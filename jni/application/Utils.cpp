#include <zenilib.h>
#include <cmath>
#include "Utils.h"

const double Utils::getAngleFromVector(const Zeni::Vector2f &vector) {
	if (vector.i == 0.0) return PI/2.0;
	double angle = atan(vector.j / vector.i);
	if (vector.i < 0) angle += PI;
	return angle;
}

const Zeni::Vector2f Utils::getVectorFromAngle(const double angle) {
	return Zeni::Vector2f(cos(angle), sin(angle));
}