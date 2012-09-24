#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <zenilib.h>
#include <limits>
#include "Tile.h"

struct Collision {
	Zeni::Point2f position;
	Zeni::Vector2f widthDistanceVector;
	Zeni::Vector2f heightDistanceVector;

	bool isColliding;

	Collision() : position(), 
		isColliding(false), 
		widthDistanceVector(Zeni::Vector2f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max())), 
		heightDistanceVector(Zeni::Vector2f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max())) {}
};

class GameObject {

private:
	Zeni::Point2f m_position; // Upper left corner
	Zeni::Vector2f m_size; // (width, height)
	Zeni::String m_image; // Name of the image to draw

public:
	GameObject(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		 const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f),
		 const Zeni::String &image = "placeholder");

	void setPosition(const Zeni::Point2f position);
	
	const Zeni::Point2f getPosition() const;
	const Zeni::Vector2f getSize() const;
	virtual const std::pair<Zeni::Point2f, Zeni::Point2f> getBoundingBox() const;
	const Zeni::Point2f getCenter() const;
	virtual const Collision getCollision(const std::list<Zeni::Point2f> &points, const bool nearest = false) const;

	virtual ~GameObject();
};
#endif

