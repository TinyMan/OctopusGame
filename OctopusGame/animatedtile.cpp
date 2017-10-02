// animatedtile.cpp
// File handling the animated tiles

#include "stdafx.h"
#include "animatedtile.h"
#include "graphics.h"

AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPositions, float duration, SDL_Texture* tileset,
	Vector2 size, Vector2 position) :
	Tile(tileset, size, tilesetPositions.at(0), position),
	_tilesetPositions(tilesetPositions),
	_duration(duration),
	_tileToDraw(0)
{

}

void AnimatedTile::update(int elapsedTime) {
	// Timer code
	if (this->_amountOfTime <= 0) {
		if (this->_tileToDraw == (int)this->_tilesetPositions.size() - 1) {
			this->_tileToDraw = 0;
		}
		else {
			this->_tileToDraw++;
		}
		this->_amountOfTime = this->_duration;
	}
	else {
		this->_amountOfTime -= elapsedTime;
	}

	Tile::update(elapsedTime);
}

void AnimatedTile::draw(GraphicalOctopus &graphics) {
	SDL_Rect destRect = { this->_position.x, this->_position.y,
			(this->_size.x * globals::TILE_SCALE), (this->_size.y * globals::TILE_SCALE) };
	SDL_Rect sourceRect = { this->_tilesetPositions.at(this->_tileToDraw).x, this->_tilesetPositions.at(this->_tileToDraw).y,
			this->_size.x, this->_size.y};
	graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
}