#ifndef ANIMATEDTILE_H
#define ANIMATEDTILE_H

#include <vector>
#include "tile.h"
#include "globals.h"

class GraphicalOctopus;

class AnimatedTile : public Tile {
public:
	AnimatedTile(std::vector<Vector2> tilesetPositions, float duration, SDL_Texture* tileset, Vector2 size, Vector2 position);
	void update(int elapsedTime);
	void draw(GraphicalOctopus &graphics);
protected:
	float _amountOfTime = 0;
	bool _notDone = false;
private:
	std::vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	float _duration;
};

struct AnimatedTileInfo {
public:
	int tilesetFirstGid;
	int startTileId;
	std::vector<int> tileIds;
	float duration;
};

#endif // !ANIMATEDTILE_H
