// slope.cpp
// File handling the informations of a slope

#include "stdafx.h"
#include "slope.h"
#include <math.h>

Slope::Slope() {

}

Slope::Slope(Vector2 p1, Vector2 p2) :
	_p1(p1),
	_p2(p2)
{
	if (this->_p2.x - this->_p1.x != 0) {
		this->_slope = (float)((fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x)));
	}
}

/* TODO: remove
const float Slope::getSlope() const {
	return this->_slope;
}
*/

const bool Slope::collidesWith(const Rectangle &other) const {
	return
			(other.getRight() >= this->_p2.x &&
			other.getLeft() <= this->_p1.x &&
			other.getTop() <= this->_p2.y &&
			other.getBottom() >= this->_p1.y)
		||
			(other.getRight() >= this->_p1.x &&
			other.getLeft() <= this->_p2.x &&
			other.getTop() <= this->_p1.y &&
			other.getBottom() >= this->_p2.y)
		||
			(other.getLeft() <= this->_p1.x &&
			other.getRight() >= this->_p2.x && 
			other.getTop() <= this->_p1.y &&
			other.getBottom() >= this->_p2.y)
		||
			(other.getLeft() <= this->_p2.x &&
			other.getRight() >= this->_p1.x &&
			other.getTop() <= this->_p2.y &&
			other.getBottom() >= this->_p1.y);
}

/* TODO: remove
const Vector2 Slope::getP1() const {
	return this->_p1;
}

const Vector2 Slope::getP2() const {
	return this->_p2;
}
*/