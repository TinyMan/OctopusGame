#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

class Rectangle {
public:
	Rectangle();
	Rectangle(int x, int y, int width, int height);

	// Getting the centers of width and height
	const int getCenterX() const;
	const int getCenterY() const;

	// Getting the position of each side
	const int getLeft() const;
	const int getRight() const;
	const int getTop() const;
	const int getBottom() const;

	// Getting the constants of the rectangle
	const int getWidth() const;
	const int getHeight() const;

	// Getting the position of a particular side
	const int getSide(const sides::Side side) const;

	// Takes in another rectangle and check if the two are colliding
	const bool collidesWith(const Rectangle &other) const;

	const bool isValidRectangle() const;

private:
	int _x, _y;
	int _width, _height;
};
#endif // !RECTANGLE_H
