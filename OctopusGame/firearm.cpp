// firearm.cpp
// File handling all informations regarding weapons

#include "stdafx.h"
#include "firearm.h"
#include "bullet.h"
#include "game.h"
#include "player.h"

FireArm::FireArm() {

}

FireArm::FireArm(GraphicalOctopus &graphics, Vector2 spawnPoint, Player* player, std::string path) :
	AnimatedSprite(graphics, "Content/Sprites/Rifle.png", 0, 0, 32, 32, spawnPoint.x, spawnPoint.y, 100),
	_player(player),
	_graphics(graphics),
	_maxAmmunition(10), /* TODO: change values, put in in CTor */
	_currentAmmunition(10),
	_lastReload(1000),
	_reloadTime(1000),
	_reloading(false),
	_lastShot(100),
	_fireRate(100),
	_bulletDirection(player->getFacing()),
	_bulletSpeed(0.2f),
	_bulletPower(4.0f),
	_bulletForce(Force(0.4f, 250))
{
	this->setupAnimations();
	this->playAnimation("IdleRight");
}

bool FireArm::shoot(Direction direction) {
	if (!this->_reloading) {
		if (this->_currentAmmunition > 0) {
			if (this->_lastShot >= this->_fireRate) {
				Bullet b = Bullet(this->_graphics, *this);
				this->_player->_game->_bullets.push_back(b);
				this->_lastShot = 0;
				this->_currentAmmunition -= 1;
				return true;
			}
		}
	}
	return false;
}

void FireArm::reload() {
	this->_reloading = true;
	this->_lastReload = 0;
	this->_currentAmmunition = this->_maxAmmunition;
}

void FireArm::update(int elapsedTime) {
	if (!this->_reloading) {
		// If we are not reloading, we check the ammunition and trigger a reload if needed
		if (this->_currentAmmunition <= 0) {
			this->reload();
		}
	}
	else {
		// If we are reloading, we check the time that passed since the reload
		if(this->_lastReload >= this->_reloadTime) {
			// If the time since reload is greater than the time taken to reload, we finish the reload
			this->_reloading = false;
		}
		else {
			// If it is not greater, we add the elapsed Time to the reload time
			this->_lastReload += elapsedTime;
		}
	}

	// We increment the time since last shot to know if we can fire again at the next iteration
	this->_lastShot += elapsedTime;

	AnimatedSprite::update(elapsedTime);
}

void FireArm::draw(GraphicalOctopus &graphics) {
	AnimatedSprite::draw(graphics, (this->_x), (this->_y));
}

void FireArm::animationDone(std::string currentAnimation) {

}

void FireArm::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleLeft", 32, 32, Vector2(0, 0));
	this->addAnimation(1, 1, 0, "IdleRight", 32, 32, Vector2(0, 0));
}

void FireArm::move(int dx, int dy, bool grounded) {
	this->_x = dx;
	if (!grounded)
		this->_y = dy;	
}

Player* FireArm::getPlayer() const {
	return this->_player;
}

const float FireArm::getBulletSpeed() const {
	return this->_bulletSpeed;
}

const Direction FireArm::getBulletDirection() const {
	return this->_bulletDirection;
}

void FireArm::setBulletDirection(Direction direction) {
	this->_bulletDirection = direction;
}

const float FireArm::getBulletPower() const {
	return this->_bulletPower;
}

const Force FireArm::getBulletForce() const {
	return this->_bulletForce;
}