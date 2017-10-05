#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "slope.h"
#include <vector>

class GraphicalOctopus;

class Player : public AnimatedSprite {
public:

	Player();
	Player(GraphicalOctopus &graphics,  Vector2 spawnPoint);
	void draw(GraphicalOctopus &graphics);
	void update(int elapsedTime);

	// Movement functions
	void moveLeft();
	void moveRight();
	void stopMoving();
	void moveTo(Vector2 position);

	// Starts jumping
	void jump();

	// Makes the player drop if he is on a platform (horizontal)
	void dropDown();

	// Logic that happens when an animation ends
	virtual void animationDone(std::string currentAnimation);

	// A required function that sets up all the animation for a sprite
	virtual void setupAnimations();

	// Handling collisions
	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);

	// Add a force to the player
	void addForce(Force f);
	// Clear all forces 
	void clearForces();

	// Getters :
	const int getId() const;
	const int getX() const;
	const int getY() const;
	const int getPreviousX() const;
	const int getPreviousY() const;
	const Direction getFacing() const;
	const Force getSumOfAllForces(int elapsedTime);

private:
	int _id;
	static int _numberOfPlayers;

	float _dx, _dy;
	int _previousX, _previousY;
	Direction _facing;

	bool _grounded;

	std::vector<Force> _forces;
};

// Operators redefinition :
bool operator==(Player const &player1, Player const &player2);
#endif // !PLAYER_H
