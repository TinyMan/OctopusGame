// player.cpp
// File handling the informations of the player

#include "stdafx.h"
#include "player.h"
#include "graphics.h"
#include "animatedsprite.h"

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}


Player::Player() {

}

Player::Player(GraphicalOctopus &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "Content/Sprites/MyCharPNG.png", 0, 0, 16, 16, (float)spawnPoint.x, (float)spawnPoint.y, 100),
	_dx(0.0f),
	_dy(0.0f),
	_facing(RIGHT),
	_grounded(false),
	_lookingUp(false),
	_lookingDown(false),
	_maxHealth(3),
	_currentHealth(3)
{
	graphics.loadImage("Content/Sprites/MyCharPNG.png");
	this->setupAnimations();
	this->playAnimation("IdleRight");
}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0,0));
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0,0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0,0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0,0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0,0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0,0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0,0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {

}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}

const float Player::getPreviousX() const {
	return this->_previousX;
}

const float Player::getPreviousY() const {
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

void Player::moveLeft() {
	if(this->_lookingDown && this->_grounded) {
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
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

void Player::lookUp() {
	this->_lookingUp = true;
	if (this->_dx == 0) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		this->playAnimation(this->_facing == LEFT ? "RunRightUp" : "RunRightUp");
	}
}

void Player::stopLookingUp() {
	this->_lookingUp = false;
}

void Player::lookDown() {
	/*
	this->_lookingDown = true;
	if (this->_grounded) {
		// We need to interact and turn backwards
		this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	} 
	else {
		this->playAnimation(this->_facing == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
	*/
	// TODO: rename this function dropDown()
	if (this->_grounded) {
		this->_y += 1;
		this->_grounded = false;
	}
}

void Player::stopLookingDown() {
	this->_lookingDown = false;
}

// Handle collisions with ALL tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	// Figure out what side the collision happened on, and move the player accordingly
	for (int i = 0; i < (int)others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
				case sides::LEFT :
					this->_x = (float) (others.at(i).getRight() + 1);
					break;
				case sides::RIGHT :
					this->_x = (float) (others.at(i).getLeft() - this->_boundingBox.getWidth() - 1);
					break;
				case sides::TOP :
					this->_dy = 0;
					this->_y = (float)(others.at(i).getBottom() + 1);
					if (this->_grounded) {
						this->_dx = 0;
						this->_x -= (this->_facing == RIGHT ? 1.0f : -1.0f);
					}
					break;
				case sides::BOTTOM :
					this->_y = (float) (others.at(i).getTop() - this->_boundingBox.getHeight() - 1);
					this->_dy = 0;
					this->_grounded = true;
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
			this->_y = (float) (newY - this->_boundingBox.getHeight());
			this->_grounded = true;
		}
	}
}

void Player::update(float elapsedTime) {
	// Apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	// Store previous location
	this->_previousX = this->_x;
	this->_previousY = this->_y;

	// Move by dx
	this->_x += this->_dx * elapsedTime;

	// Move by dy
	this->_y += this->_dy * elapsedTime;

	AnimatedSprite::update(elapsedTime);
}

void Player::draw(GraphicalOctopus &graphics) {
	AnimatedSprite::draw(graphics, (int)(this->_x), (int)(this->_y));
}