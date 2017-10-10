#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "animatedsprite.h"
#include "slope.h"
#include <vector>

class GraphicalOctopus;
class Game;
class FireArm;

class Player : public AnimatedSprite {
public:

	// TODO: remove when bullet provider is here
	Game* _game;

	Player();
	Player(GraphicalOctopus &graphics,  Vector2 spawnPoint, Game* game);
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

	// Removes a life from the player
	// Returns true if the life count is still > 0, false otherwise
	bool loseALife();

	// Firing a weapon
	void shoot();

	// Getters :
	const int getId() const;
	const int getX() const;
	const int getY() const;
	const int getPreviousX() const;
	const int getPreviousY() const;
	const Direction getFacing() const;
	const Force getSumOfAllForces(int elapsedTime);
	const int getLivesLeft() const;

private:
	// Identifier
	static int _numberOfPlayers;
	int _id;

	// Number of lives left
	int _lives;

	// Movement attributes
	float _dx, _dy;
	int _previousX, _previousY;
	std::vector<Force> _forces; /* Forces that are applied to the player (via Bullets for example) */
	
	// Position attributes
	Direction _facing;
	bool _grounded;

	// Weapons 
	FireArm* _primary;
	FireArm* _secondary;
	std::string _currentFireArm;

};

// Operators redefinition :
bool operator==(Player const &player1, Player const &player2);

#endif // !PLAYER_H
