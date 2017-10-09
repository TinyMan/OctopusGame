#ifndef HUD_H
#define HUD_H

#include "sprite.h"
#include "player.h"
#include <vector>

class GraphicalOctopus;

class HUD {
public:
	HUD();
	HUD(GraphicalOctopus &graphics, Player* player1, Player* player2);

	void update(int elapsedTime);
	void draw(GraphicalOctopus &graphics);

private:
	// TODO: remove unecessary comments
	//Player _player;
	std::vector<Player*> _players;

	// Health sprites
	//Sprite _healthBarSprite, _healthNumber, _currentHealthBar;
	std::vector<Sprite> _scores;
};

#endif // !HUD_H
