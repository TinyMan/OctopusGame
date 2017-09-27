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
private:
	void gameLoop();
	void draw(GraphicalOctopus &graphics);
	void update(float elapsedTime);

	std::vector<Player> _players;

	Player _player;
	Player _player2;
	Level _level;
	std::vector<Bullet> _bullets;

	HUD _hud;
};
#endif // !GAME_H
