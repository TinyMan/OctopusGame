#ifndef SLOPE_H
#define SLOPE_H

#include "globals.h"
#include "rectangle.h"

class Slope {
public:
	Slope();
	Slope(Vector2 p1, Vector2 p2);

	const inline float getSlope() const {
		return this->_slope;
	}

	const bool collidesWith(const Rectangle &other) const;

	const inline Vector2 getP1() const {
		return this->_p1;
	}

	const inline Vector2 getP2() const {
		return this->_p2;
	}
private:
	Vector2 _p1, _p2;
	float _slope;
};

#endif // !SLOPE_H
