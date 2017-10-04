// player.cpp
// File handling the informations of the player

#include "stdafx.h"
#include "player.h"
#include "graphics.h"
#include <algorithm>

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

int Player::_numberOfPlayers = 0;

Player::Player() {

}

Player::Player(GraphicalOctopus &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "Content/Sprites/BlobSprite.png", 0, 0, 32, 32, spawnPoint.x, spawnPoint.y, 100),
	_dx(0.0f),
	_dy(0.0f),
	_facing(RIGHT),
	_grounded(false),
	_lookingUp(false),
	_lookingDown(false),
	_maxHealth(3),
	_currentHealth(3)
{
	this->_numberOfPlayers++;
	this->_id = this->_numberOfPlayers;

	graphics.loadImage("Content/Sprites/BlobSprite.png");
	this->setupAnimations();
	this->playAnimation("IdleRight");
}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleLeft", 32, 32, Vector2(0, 0));
	this->addAnimation(1, 0, 32, "IdleRight", 32, 32, Vector2(0, 0));
	this->addAnimation(3, 0, 0, "RunLeft", 32, 32, Vector2(0, 0));
	this->addAnimation(3, 0, 32, "RunRight", 32, 32, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "IdleLeftUp", 32, 32, Vector2(0,0));
	this->addAnimation(1, 3, 32, "IdleRightUp", 32, 32, Vector2(0,0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 32, 32, Vector2(0,0));
	this->addAnimation(3, 3, 32, "RunRightUp", 32, 32, Vector2(0,0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 32, 32, Vector2(0,0));
	this->addAnimation(1, 6, 32, "LookDownRight", 32, 32, Vector2(0,0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 32, 32, Vector2(0,0));
	this->addAnimation(1, 7, 32, "LookBackwardsRight", 32, 32, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {

}

const int Player::getX() const {
	return this->_x;
}

const int Player::getY() const {
	return this->_y;
}

const int Player::getPreviousX() const {
	return this->_previousX;
}

const int Player::getPreviousY() const {
	return this->_previousY;
}

const Direction Player::getFacing() const {
	return this->_facing;
}

const int Player::getMaxHealth() const {
	return this->_maxHealth;
}

const int Player::getCurrentHealth() const {
	return this->_currentHealth;
}

const Force Player::getSumOfAllForces(int elapsedTime) {
	Force f = Force(0, 0);
	for (int i = 0; i < (int)this->_forces.size(); i++) {
		if (this->_forces.at(i).timeLeft <= 0) {
			this->_forces.erase(this->_forces.begin() + i--);
		}
		else {
			f.direction.x += this->_forces.at(i).direction.x;
			f.direction.y += this->_forces.at(i).direction.y;

			// Subtracting the time that passed from the Force
			this->_forces.at(i).timeLeft -= elapsedTime;
		}
	}
	return f;
}

void Player::moveLeft() {
	if (this->_lookingDown && this->_grounded) {
		return;
	}
	this->_dx = -player_constants::WALK_SPEED;
	if (this->_lookingUp) {
		this->playAnimation("RunLeftUp");
	}
	else {
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight() {
	if (this->_lookingDown && this->_grounded) {
		return;
	}
	this->_dx = player_constants::WALK_SPEED;
	if (this->_lookingUp) {
		this->playAnimation("RunRightUp");
	}
	else {
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	if (!this->_lookingUp && !this->_lookingDown) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
	}
}

void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0.0f;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

void Player::lookUp() {
	// TODO: remove UNUSED
	this->_lookingUp = true;
	if (this->_dx == 0.0f) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		this->playAnimation(this->_facing == LEFT ? "RunRightUp" : "RunRightUp");
	}
}

void Player::stopLookingUp() {
	// TODO: remove UNUSED
	this->_lookingUp = false;
}


void Player::lookDown() {
	// TODO: remove UNUSED
	this->_lookingDown = true;
	if (this->_grounded) {
		// We need to interact and turn backwards
		this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}


void Player::dropDown() {
	if (this->_grounded) {
		this->_y += 1;
		this->_grounded = false;
	}
}

void Player::stopLookingDown() {
	// TODO : remove UNUSED
	this->_lookingDown = false;
}

// Handle collisions with ALL tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	// Figure out what side the collision happened on, and move the player accordingly
	for (int i = 0; i < (int)others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
				case sides::LEFT:
					this->_x = (others.at(i).getRight() + 1);
					for (int i = 0; i < (int)this->_forces.size(); i++) {
						if (this->_forces.at(i).direction.x <= 0.0f)
							this->_forces.at(i).direction.x = - this->_forces.at(i).direction.x * globals::FORCE_REDUCTION;
					}
					break;
				case sides::RIGHT :
					this->_x = (others.at(i).getLeft() - this->_boundingBox.getWidth() - 1);
					for (int i = 0; i < (int)this->_forces.size(); i++) {
						if (this->_forces.at(i).direction.x >= 0.0f)
							this->_forces.at(i).direction.x = - this->_forces.at(i).direction.x * globals::FORCE_REDUCTION;
					}
					break;
				case sides::TOP :
					this->_dy = 0.0f;
					this->_y = (others.at(i).getBottom() + 1);
					if (this->_grounded) {
						this->_dx = 0.0f;
						this->_x -= (this->_facing == RIGHT ? 1 : -1);
					}

					for (int i = 0; i < (int)this->_forces.size(); i++) {
						if (this->_forces.at(i).direction.y <= 0.0f)
							this->_forces.at(i).direction.y = - this->_forces.at(i).direction.y * globals::FORCE_REDUCTION;
					}
					break;
				case sides::BOTTOM :
					this->_y = (others.at(i).getTop() - this->_boundingBox.getHeight() - 1);
					this->_dy = 0.0f;
					this->_grounded = true;

					for (int i = 0; i < (int)this->_forces.size(); i++) {
						if (this->_forces.at(i).direction.y >= 0.0f)
							this->_forces.at(i).direction.y = - this->_forces.at(i).direction.y * globals::FORCE_REDUCTION;
					}
					break;
			}
		}
	}
}

// Handles collisions with all slopes the player is colliding with
void Player::handleSlopeCollisions(std::vector<Slope> &others) {
	for (int i = 0; i < (int)others.size(); i++) {
		// Calculate where on the slope the player's bottom center is colliding	
		// and use y = mx+b to figure out the y position to place him at
		// First calculate b (slope intercept) using one of the points (b = y - mx)
		int b = (int)(others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		// Now get the player's center x
		int centerX = this->_boundingBox.getCenterX();

		// Now pass that x into the equation y = mx+b using newly found b get the y position
		int newY = (int)(others.at(i).getSlope() * centerX + b) - 8; // 8 is temporary to fix a problem

		// Re-position the player to the correct y
		if (this->_grounded) {
			this->_y = (newY - this->_boundingBox.getHeight());
			this->_grounded = true;
		}
	}
}

void Player::update(int elapsedTime) {
	// Apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	// Store previous location
	this->_previousX = this->_x;
	this->_previousY = this->_y;

	// Compute the force applied on the player
	Force allForces = getSumOfAllForces(elapsedTime);

	// Move by dx
	this->_x += (int) (this->_dx * elapsedTime);

	// Move by dy
	this->_y += (int) (this->_dy * elapsedTime);

	// Move by force
	this->_x += (int) (allForces.direction.x * elapsedTime);
	this->_y += (int) (allForces.direction.y * elapsedTime);

	AnimatedSprite::update(elapsedTime);
}

void Player::draw(GraphicalOctopus &graphics) {
	AnimatedSprite::draw(graphics, (this->_x), (this->_y));
}

const int Player::getId() const {
	return this->_id;
}

bool operator==(const Player &player1, const Player &bullet2) {
	return (player1.getId() == bullet2.getId());
}