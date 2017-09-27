#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "slope.h"

class GraphicalOctopus;

class Player : public AnimatedSprite {
public:
	Player();
	Player(GraphicalOctopus &graphics,  Vector2 spawnPoint);
	void draw(GraphicalOctopus &graphics);
	void update(float elapsedTime);

	// Move the player left by -dx
	void moveLeft();

	// Move the player right by dx
	void moveRight();

	// Stops the movement of the player
	void stopMoving();

	// Starts jumping
	void jump();

	// Make the player look up
	void lookUp();

	// Stops the player from looking up
	void stopLookingUp();

	// Make the player look down OR interact
	void lookDown();

	// Makes the player drop if he is on a platform (horizontal)
	void dropDown();
	
	// Stops the player from looking down
	void stopLookingDown();

	// Logic that happens when an animation ends
	virtual void animationDone(std::string currentAnimation);

	// A required function that sets up all the animation for a sprite
	virtual void setupAnimations();

	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);

	// Getters :
	const float getX() const;
	const float getY() const;
	const float getPreviousX() const;
	const float getPreviousY() const;
	const Direction getFacing() const;

	const int getMaxHealth() const;
	const int getCurrentHealth() const;

private:
	float _dx, _dy;
	float _previousX, _previousY;
	Direction _facing;

	bool _grounded;

	bool _lookingUp, _lookingDown;

	int _maxHealth, _currentHealth;
};
#endif // !PLAYER_H
