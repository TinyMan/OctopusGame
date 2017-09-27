#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "rectangle.h"
#include "globals.h"

class GraphicalOctopus;

class Sprite {
public:
	Sprite();
	Sprite(GraphicalOctopus &graphics, const std::string &filePath, int sourceX, int sourceY,
		int width, int height, float posX, float posY);
	virtual ~Sprite();
	virtual void update();
	void draw(GraphicalOctopus &graphics, int x, int y);

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle &other) const;

	const float getX() const;
	const float getY() const;

	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);

protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
	float _x, _y;

	Rectangle _boundingBox;
};

#endif // !SPRITE_H
