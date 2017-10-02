#ifndef HUD_H
#define HUD_H

#include "sprite.h"
#include "player.h"

class GraphicalOctopus;

class HUD {
public:
	HUD();
	HUD(GraphicalOctopus &graphics, Player &player);

	void update(int elapsedTime);
	void draw(GraphicalOctopus &graphics);

private:
	Player _player;

	// Health sprites
	Sprite _healthBarSprite, _healthNumber, _currentHealthBar;
};

#endif // !HUD_H
