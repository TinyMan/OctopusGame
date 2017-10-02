#ifndef TILE_H
#define TILE

#include "globals.h"

struct SDL_Texture;
class GraphicalOctopus;

class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	void update(int elapsedTime);
	void draw(GraphicalOctopus &graphics);

protected:
	SDL_Texture* _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;

private:

};
#endif // !TILE_H
