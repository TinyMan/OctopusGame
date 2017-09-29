#ifndef BULLET_H
#define BULLET_H

#include "animatedsprite.h"
#include "player.h"
#include <string>

class GraphicalOctopus;

class Bullet : public AnimatedSprite {
public:
	Bullet();
	Bullet(GraphicalOctopus &graphics, Vector2 spawnPoint, Direction facing, float speed);
	Bullet(GraphicalOctopus &graphics, Player &player, float speed);
	void draw(GraphicalOctopus &graphics);
	void update(float elapsedTime);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	const Player getPlayer() const;

	// handle collisions with tiles and players
protected:
	// Has a player that it cannot harm
	Player _player;
	Direction _facing;
	float _speed;
private:
};

#endif // !BULLET_H
