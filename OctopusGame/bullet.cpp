// bullet.cpp
// File handling all the informations of the in-game bullets

#include "stdafx.h"
#include "bullet.h"
#include "graphics.h"
#include "firearm.h"

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

Bullet::Bullet(GraphicalOctopus &graphics, Player *player, float speed, Force force) :
	Bullet(graphics, Vector2((player->getFacing() == LEFT ? player->getX() : (player->getX() + player->getBoundingBox().getWidth() - 2)), 
	(player->getY() + player->getBoundingBox().getHeight()/2)), player->getFacing(), speed)
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

Bullet::Bullet(GraphicalOctopus &graphics, FireArm &firearm) :
	Bullet(graphics, Vector2(firearm.getX(), firearm.getY()), firearm.getBulletDirection(), firearm.getBulletSpeed()) 
{
	this->_player = firearm.getPlayer();
	this->_force = Force(0.4f, 250); // TODO: change force direction depending on facing 
	this->_force.direction = Vector2f(1.0f * this->_force.power, 0.0f);
}

void Bullet::draw(GraphicalOctopus &graphics) {
	AnimatedSprite::draw(graphics, (this->_x), (this->_y));
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

const Player* Bullet::getPlayer() const {
	return this->_player;
}

Force Bullet::getForce() const {
	return this->_force;
}