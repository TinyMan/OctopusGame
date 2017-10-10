// hud.cpp 
// File handling the Heads Up Display of the game

#include "stdafx.h"
#include "hud.h"
#include "graphics.h"

HUD::HUD() {

}

HUD::HUD(GraphicalOctopus &graphics, Player* player1, Player* player2){
	// TODO: remove unecessary comments
	this->_players.push_back(player1);
	this->_players.push_back(player2);

	//graphics.loadImage("Content/Sprites/TextBox.png");

	//this->_healthBarSprite = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 40, 64, 8, 35, 70);
	this->_scores.push_back(Sprite(graphics, "Content/Sprites/TextBox.png", 0, 56, 8, 8, 66, 70));
	this->_scores.push_back(Sprite(graphics, "Content/Sprites/TextBox.png", 0, 56, 8, 8, globals::SCREEN_WIDTH - 66, 70));
	//this->_currentHealthBar = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 25, 39, 5, 83, 72);
}

void HUD::update(int elapsedTime) {
	for (int i = 0; i < (int)this->_players.size(); i++) {
		this->_scores.at(i).setSourceRectX(8 * (this->_players.at(i)->getLivesLeft()));
	}

	// Calculate the width of the health bar (full = 39 px)
	//float num = (float)this->_player.getCurrentHealth() / this->_player.getMaxHealth();
	//this->_currentHealthBar.setSourceRectW((int)std::floor(num * 39));
}

void HUD::draw(GraphicalOctopus &graphics) {
	for (int i = 0; i < (int)this->_players.size(); i++) {
		this->_scores.at(i).draw(graphics, (int)(this->_scores.at(i).getX()), (int)(this->_scores.at(i).getY()));
	}
	//this->_healthBarSprite.draw(graphics, (int)(this->_healthBarSprite.getX()), (int)(this->_healthBarSprite.getY()));
	//this->_currentHealthBar.draw(graphics, (int)(this->_currentHealthBar.getX()), (int)(this->_currentHealthBar.getY()));
}

