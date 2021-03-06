// sprite.cpp
// File handling individual sprites

#include "stdafx.h"
#include "sprite.h"
#include "graphics.h"
#include "globals.h"

Sprite::Sprite() {

}

Sprite::Sprite(GraphicalOctopus &graphics, const std::string &filePath, int sourceX, int sourceY,
	int width, int height, int posX, int posY) :
	_x(posX),
	_y(posY)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	this->_spriteSheet = graphics.loadImage(filePath);

	if (this->_spriteSheet == NULL) {
		printf("Error : Unable to load image \"%s\"\n", filePath.c_str());
	}

	this->_boundingBox = Rectangle(this->_x, this->_y, (width * globals::SPRITE_SCALE), 
		(height * globals::SPRITE_SCALE));
}

Sprite::~Sprite() {

}

void Sprite::draw(GraphicalOctopus &graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, (this->_sourceRect.w * globals::SPRITE_SCALE), 
		(this->_sourceRect.h * globals::SPRITE_SCALE) };
	graphics.blitSurface(_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {
	this->_boundingBox = Rectangle(this->_x, this->_y, (this->_sourceRect.w * globals::SPRITE_SCALE), 
		(this->_sourceRect.h * globals::SPRITE_SCALE));
}

const Rectangle Sprite::getBoundingBox() const {
	return this->_boundingBox;
}

// Determine which side the collision happened on
const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->getBoundingBox().getRight() - other.getLeft();
	amtLeft = other.getRight() - this->getBoundingBox().getLeft();
	amtTop = other.getBottom() - this->getBoundingBox().getTop();
	amtBottom = this->getBoundingBox().getBottom() - other.getTop();

	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 0; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}

	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}

const int Sprite::getX() const {
	return this->_x;
}

const int Sprite::getY() const {
	return this->_y;
}

void Sprite::setSourceRectX(int value) {
	this->_sourceRect.x = value;
}

void Sprite::setSourceRectY(int value) {
	this->_sourceRect.y = value;
}

void Sprite::setSourceRectW(int value) {
	this->_sourceRect.w = value;
}

void Sprite::setSourceRectH(int value) {
	this->_sourceRect.h = value;
}