#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include <string>
#include <vector>
#include "rectangle.h"
#include "slope.h"
#include "animatedtile.h"

class GraphicalOctopus;
class Player;
class Bullet;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	Level(std::string mapName, Vector2 spawnPoint, GraphicalOctopus &graphics);
	~Level();
	void update(float elapsedTime);
	void draw(GraphicalOctopus &graphics);

	std::vector<Rectangle> checkTileCollision(Player &player);
	std::vector<Rectangle> checkBulletCollision(const Rectangle &other);
	std::vector<Slope> checkSlopeCollision(const Rectangle &other);

	const Vector2 getPlayerSpawnPoint() const;

private:
	std::string _mapName;
	Vector2 _spawnPoint;

	Vector2 _size;
	Vector2 _tileSize;

	SDL_Texture* _backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _verticalCollisionRects;
	std::vector<Rectangle> _horizontalCollisionRects;

	std::vector<Slope> _slopes;
	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfos;

	// Loads a map
	void loadMap(std::string mapName, GraphicalOctopus &graphics);	

	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
};

// Tileset structure 
struct Tileset {
	SDL_Texture* texture;
	int firstGid;
	
	Tileset() {
		this->firstGid = -1;
	}

	Tileset(SDL_Texture* texture, int firstGid) {
		this->texture = texture;
		this->firstGid = firstGid;
	}
};

#endif // !LEVEL_H
