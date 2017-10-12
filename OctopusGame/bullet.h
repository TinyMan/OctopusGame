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
	Bullet(GraphicalOctopus &graphics, Player *player, float speed, Force force);
	Bullet(GraphicalOctopus &graphics, FireArm &firearm);
	void draw(GraphicalOctopus &graphics);
	void update(int elapsedTime);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	// Getters :
	const Player* getPlayer() const;
	Force getForce() const;

	// handle collisions with tiles and players
protected:
	
private:
	// Has a player that it cannot harm
	Player* _player;
	Direction _facing;
	float _speed;
	Force _force;
};

#endif // !BULLET_H
