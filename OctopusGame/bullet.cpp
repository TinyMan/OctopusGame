// bullet.cpp
// File handling all the informations of the in-game bullets

#include "stdafx.h"
#include "bullet.h"
#include "graphics.h"

Bullet::Bullet() {

}

Bullet::Bullet(GraphicalOctopus &graphics, Vector2 spawnPoint, Direction facing, float speed) :
	AnimatedSprite(graphics, "Content/Sprites/BulletV1.png", 0, 0, 10, 10, spawnPoint.x, spawnPoint.y, 100),
	_facing(facing),
	_speed(speed)
{
	graphics.loadImage("Content/Sprites/BulletV1.png");
	this->setupAnimations();
	if (this->_facing == RIGHT) {
		this->playAnimation("BulletRight");
	}
	else {
		this->playAnimation("BulletLeft");
	}
}

Bullet::Bullet(GraphicalOctopus &graphics, Player &player, float speed, Force force) :
	Bullet(graphics, Vector2((int)(player.getX() + player.getBoundingBox().getWidth()/2), 
	(int)(player.getY() + player.getBoundingBox().getHeight()/2)), player.getFacing(), speed)
{
	this->_player = player;
	if (this->_facing == RIGHT) {
		force.direction = Vector2f(1.0f * force.power, 0.0f);
	}
	else {
		force.direction = Vector2f(-1.0f * force.power, 0.0f);
	}
	this->_force = force;
}

void Bullet::draw(GraphicalOctopus &graphics) {
	AnimatedSprite::draw(graphics, (int)(this->_x), (int)(this->_y));
}

void Bullet::update(int elapsedTime) {
	if (_facing == RIGHT) {
		this->_x += (int)(_speed * elapsedTime);
	}
	else {
		this->_x -= (int)(_speed * elapsedTime);
	}

	AnimatedSprite::update(elapsedTime);
}

void Bullet::animationDone(std::string currentAnimation) {

}

void Bullet::setupAnimations() {
	this->addAnimation(1, 0, 0, "BulletRight", 9, 9, Vector2(0, 0));
	this->addAnimation(1, 1, 0, "BulletLeft", 9, 9 , Vector2(0, 0));
}

const Player& Bullet::getPlayer() const {
	return this->_player;
}

Force Bullet::getForce() const {
	return this->_force;
}