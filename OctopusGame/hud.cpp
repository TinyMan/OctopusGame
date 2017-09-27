// hud.cpp 
// File handling the Heads Up Display of the game

#include "stdafx.h"
#include "hud.h"
#include "graphics.h"

HUD::HUD() {

}

HUD::HUD(GraphicalOctopus &graphics, Player &player) {
	this->_player = player;	
	this->_healthBarSprite = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 40, 64, 8, 35, 70);
	this->_healthNumber = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 56, 8, 8, 66, 70);
	this->_currentHealthBar = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 25, 39, 5, 83, 72);
}

void HUD::update(float elapsedTime) {
	this->_healthNumber.setSourceRectX(8 * this->_player.getCurrentHealth());

	// Calculate the width of the health bar (full = 39 px)
	float num = (float)this->_player.getCurrentHealth() / this->_player.getMaxHealth();
	this->_currentHealthBar.setSourceRectW((int)std::floor(num * 39));
}

void HUD::draw(GraphicalOctopus &graphics) {
	this->_healthBarSprite.draw(graphics, (int)(this->_healthBarSprite.getX()), (int)(this->_healthBarSprite.getY()));
	this->_healthNumber.draw(graphics, (int)(this->_healthNumber.getX()), (int)(this->_healthNumber.getY()));
	this->_currentHealthBar.draw(graphics, (int)(this->_currentHealthBar.getX()), (int)(this->_currentHealthBar.getY()));
}

