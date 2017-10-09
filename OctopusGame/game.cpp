// game.cpp
// File handling the main game loop

#include "stdafx.h"
#include <SDL.h>
#include <algorithm>
#include "game.h"
#include "graphics.h"
#include "input.h"

#include <iostream>

namespace {
	const Uint32 FPS = 50;
	const Uint32 MAX_FRAME_TIME = 1000 / FPS;
}

// Constructor
Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	this->_paused = false;
	this->gameLoop();
}

// Destructor
Game::~Game() {

}

void Game::gameLoop() {
	GraphicalOctopus graphics;
	Input input;
	SDL_Event event;

	// Creating the level
	this->_level = Level("TestMap", Vector2(100, 100), graphics);

	// Adding two players
	this->_players.push_back(Player(graphics, this->_level.getPlayerSpawnPoints().at(0)));
	this->_players.push_back(Player(graphics, this->_level.getPlayerSpawnPoints().at(1)));

	this->_hud = HUD(graphics, &this->_players.at(0), &this->_players.at(1));	

	int updatedTime = 0;

	// Start of the game loop
	while (true) {
		input.beginNewFrame();
		const Uint32 now = SDL_GetTicks();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return;
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			else if (event.type = SDL_KEYUP) {
				input.keyUpEvent(event);
			}
		}
		// Quittting the game
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
			return;
		}

		// Moving left and right
		//		- first player
		if (input.isKeyHeld(SDL_SCANCODE_LEFT)) {
			this->_players.at(0).moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_players.at(0).moveRight();
		}
		//		- second player
		if (input.isKeyHeld(SDL_SCANCODE_A)) {
			this->_players.at(1).moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_D)) {
			this->_players.at(1).moveRight();
		}

		// Dropping down and jumping
		//		- first player
		if (input.wasKeyPressed(SDL_SCANCODE_DOWN) || input.isKeyHeld(SDL_SCANCODE_DOWN)) {
			this->_players.at(0).dropDown();
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_UP) || input.isKeyHeld(SDL_SCANCODE_UP)) {
			this->_players.at(0).jump();
		}
		//		- second player
		if (input.wasKeyPressed(SDL_SCANCODE_S) || input.isKeyHeld(SDL_SCANCODE_S)) {
			this->_players.at(1).dropDown();
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_W) || input.isKeyHeld(SDL_SCANCODE_W)) {
			this->_players.at(1).jump();
		}

		// Firing weapon
		//		- first player
		if (input.wasKeyPressed(SDL_SCANCODE_KP_4)) {
			this->_bullets.push_back(Bullet(graphics, this->_players.at(0), 0.2f, Force(0.4f, 250)));
		}
		//		- second player
		if (input.isKeyHeld(SDL_SCANCODE_G)) {
			this->_bullets.push_back(Bullet(graphics, this->_players.at(1), 0.2f, Force(0.4f, 250)));
		}

		// Stop motion
		//		- first player
		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_players.at(0).stopMoving();
		}
		//		- second player
		if (!input.isKeyHeld(SDL_SCANCODE_A) && !input.isKeyHeld(SDL_SCANCODE_D)) {
			this->_players.at(1).stopMoving();
		}

		// Pause/Unpause the game
		if (input.wasKeyPressed(SDL_SCANCODE_SPACE)) {
			this->_paused = !this->_paused;
		}

		const auto delta = now - updatedTime;
		if (delta > MAX_FRAME_TIME) {
			this->update(MAX_FRAME_TIME);
			this->draw(graphics);
			updatedTime += MAX_FRAME_TIME;
		}
		else
			SDL_Delay(MAX_FRAME_TIME - delta);


	} // End of the game loop
}

void Game::draw(GraphicalOctopus &graphics) {

	// Clearing all previous graphics
	graphics.clear();

	// Drawing the level
	this->_level.draw(graphics);

	// Drawing the players
	for (int i = 0; i < (int)this->_players.size(); i++) {
		this->_players.at(i).draw(graphics);
	}

	// Drawing the bullets
	for (int i = 0; i < (int)this->_bullets.size(); i++) {
		this->_bullets.at(i).draw(graphics);
	}

	// Drawing the HUD
	this->_hud.draw(graphics);

	// Rendering everything to the screen
	graphics.flip();
}

void Game::update(int elapsedTime) {
	if (!this->_paused) {
		// Updating players position
		for (int i = 0; i < (int)this->_players.size(); i++) {
			this->_players.at(i).update(elapsedTime);
			if (this->_players.at(i).getY() >= globals::SCREEN_HEIGHT + 200) {
				this->_players.at(i).moveTo(this->_level.getPlayerSpawnPoints().at(i));
				if (!this->_players.at(i).loseALife()) {
					this->_paused = true;
				}
			}
		}

		// Updating the level (Animated level only)
		this->_level.update(elapsedTime);

		// Updating bullet position and destroy them if they're out of bound
		for (int i = 0; i < (int)this->_bullets.size(); i++) {
			this->_bullets.at(i).update(elapsedTime);
			if (this->_bullets.at(i).getX() > globals::SCREEN_WIDTH || this->_bullets.at(i).getX() < 0) {
				this->_bullets.erase(this->_bullets.begin() + i--);
			}
		}

		// Updating the HUD
		this->_hud.update(elapsedTime);

		// Check players collisions with tiles
		for (int i = 0; i < (int)this->_players.size(); i++) {
			std::vector<Rectangle> others;
			if ((others = this->_level.checkTileCollision(this->_players.at(i))).size() > 0) {
				// If the player is colliding with at least one tile, handle it	
				this->_players.at(i).handleTileCollisions(others);
			}
		}

		// Check projectile collisions with tiles
		for (int i = 0; i < (int)this->_bullets.size(); i++) {
			std::vector<Rectangle> others;
			if ((others = this->_level.checkBulletCollision(this->_bullets.at(i).getBoundingBox())).size() > 0) {
				this->_bullets.erase(this->_bullets.begin() + i--);
			}
		}

		// Check projectile collisions with players
		for (int i = 0; i < (int)this->_bullets.size(); i++) {
			for (int j = 0; j < (int)this->_players.size(); j++) {
				if (this->_bullets.at(i).getBoundingBox().collidesWith(this->_players.at(j).getBoundingBox())
					&& !(this->_bullets.at(i).getPlayer() == this->_players.at(j))) {
					// TODO: remove all forces except blast if exists ?
					this->_players.at(j).clearForces();

					this->_players.at(j).addForce(this->_bullets.at(i).getForce());
					this->_bullets.erase(this->_bullets.begin() + i--);
					break;
				}
			}
		}

		// Check player collisions with slopes
		for (int i = 0; i < (int)this->_players.size(); i++) {
			std::vector<Slope> otherSlopes;
			if ((otherSlopes = this->_level.checkSlopeCollision(this->_players.at(i).getBoundingBox())).size() > 0) {
				this->_players.at(i).handleSlopeCollisions(otherSlopes);
			}
		}
	}
}