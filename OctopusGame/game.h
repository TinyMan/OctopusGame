#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "level.h"
#include "bullet.h"
#include "hud.h"

class GraphicalOctopus;

class Game {
public:
	Game();
	~Game();

	// TODO: modify when bullet provider is here (--> private or delete)
	std::vector<Bullet> _bullets;
private:
	void gameLoop();
	void draw(GraphicalOctopus &graphics);
	void update(int elapsedTime);

	std::vector<Player> _players;

	Level _level;
	

	HUD _hud;
	bool _paused;
};
#endif // !GAME_H
