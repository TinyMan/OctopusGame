#ifndef FIREARM_H
#define FIREARM_H

#include "player.h"
#include "graphics.h"
#include <string>

class FireArm : public AnimatedSprite {
public:
	// Constructor
	FireArm();
	FireArm(GraphicalOctopus &graphics, Vector2 spawnPoint, Player* player, std::string path = "Content/Sprites/Rifle.png");

	// Fire the weapon
	bool shoot(Direction direction);

	// Reload
	void reload();

	// Moving the firearm
	void move(int dx, int dy, bool grounded);

	void update(int elapsedTime);
	void draw(GraphicalOctopus &graphics);
	void animationDone(std::string currentAnimation);
	void setupAnimations();

	// Getters :
	Player getPlayer() const;
	const float getBulletSpeed() const;
	const Direction getBulletDirection() const;
	const float getBulletPower() const;
	const Force getBulletForce() const;

	// Setters :
	void FireArm::setBulletDirection(Direction direction);

protected:

private:
	// Player that owns the firearm
	Player* _player;

	// Name of the firearm
	std::string _name;

	// TODO: decide if I need to store the Vector2 position of the tip of the firearm

	// Moving the firearm
	float _dx;
	float _dy;

	// Reload : time taken to reload, time of the last reload and boolean 
	int _reloadTime;
	int _lastReload;
	bool _reloading;

	// Ammunitions
	int _maxAmmunition;
	int _currentAmmunition;
	
	// Attack speed
	int _fireRate;
	int _lastShot;

	// Bullet attributes
	float _bulletSpeed;
	Direction _bulletDirection;
	float _bulletPower;
	Force _bulletForce;

	// Graphics to create bullets
	GraphicalOctopus _graphics;
};

#endif // !WEAPON_H
