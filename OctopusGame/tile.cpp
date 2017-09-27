// tile.cpp
// File handling the tiles

#include "stdafx.h"
#include "tile.h"
#include "graphics.h"
#include <SDL.h>

Tile::Tile() {

}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2((int) (position.x * globals::SPRITE_SCALE), (int)(position.y * globals::SPRITE_SCALE))) {

}

void Tile::update(float elapsedTime) {

}

void Tile::draw(GraphicalOctopus &graphics) {
	SDL_Rect destRect = { this->_position.x, this->_position.y,	(int)(this->_size.x * globals::SPRITE_SCALE),
							(int)(this->_size.y * globals::SPRITE_SCALE) };
	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };

	graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
}