#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <zenilib.h>
#include <limits>

struct Collision {
	Zeni::Point2f position;
	Zeni::Vector2f minimumSeparation;

	bool isColliding;

	Collision() : position(), 
		isColliding(false), 
		minimumSeparation(Zeni::Vector2f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max())) {}
};

class GameObject {

private:
	Zeni::Point2f m_position; // Upper left corner
	Zeni::Vector2f m_size; // (width, height)

public:
	GameObject(const Zeni::Point2f &position = Zeni::Point2f(0.0f, 0.0f),
		 const Zeni::Vector2f &size = Zeni::Vector2f(32.0f, 32.0f));

	void setPosition(const Zeni::Point2f position);
	
	const Zeni::Point2f getPosition() const;
	const Zeni::Vector2f getSize() const;
	virtual const std::pair<Zeni::Point2f, Zeni::Point2f> getBoundingBox() const;
	virtual const std::list<Zeni::Point2f> GameObject::getBoundingPoints() const;
	const Zeni::Point2f getCenter() const;
	virtual const Collision getCollision(const std::list<Zeni::Point2f> &points, const bool nearest = false) const;

};
#endif

