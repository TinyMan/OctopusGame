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
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

// Constructor
Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
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
	this->_players.push_back(Player(graphics, this->_level.getPlayerSpawnPoint()));
	this->_players.push_back(Player(graphics, this->_level.getPlayerSpawnPoint()));

	this->_hud = HUD(graphics, this->_players.at(1));

	int LAST_UPDATE_TIME = SDL_GetTicks();

	// Start of the game loop
	while (true) {
		input.beginNewFrame();

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
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
			return;

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

		//////////////// TODO: remove, UNUSED ////////////////////////////////////////
		// Looking up
		if (input.isKeyHeld(SDL_SCANCODE_UP)) {
			this->_players.at(0).lookUp();
		}

		// Stop looking up
		if (input.wasKeyReleased(SDL_SCANCODE_UP)) {
			this->_players.at(0).stopLookingUp();
		}
		// Stop looking down
		if (input.wasKeyReleased(SDL_SCANCODE_DOWN)) {
			this->_players.at(0).stopLookingDown();
		}
		//////////////// TODO: remove, UNUSED ////////////////////////////////////////

		// Dropping down and jumping
		//		- first player
			if (input.wasKeyPressed(SDL_SCANCODE_DOWN) || input.isKeyHeld(SDL_SCANCODE_DOWN)) {
				this->_players.at(0).dropDown();
			} 
			else if (input.wasKeyPressed(SDL_SCANCODE_UP)) {
				this->_players.at(0).jump();
			}
		//		- second player
			if (input.wasKeyPressed(SDL_SCANCODE_S) || input.isKeyHeld(SDL_SCANCODE_S)) {
				this->_players.at(1).dropDown();
			}
			else if (input.wasKeyPressed(SDL_SCANCODE_W)) {
				this->_players.at(1).jump();
			}

		// Firing weapon
		//		- first player
			if (input.wasKeyPressed(SDL_SCANCODE_KP_4)) {
				this->_bullets.push_back(Bullet(graphics, Vector2((int)this->_players.at(0).getX(), (int)this->_players.at(0).getY()),
					this->_players.at(0).getFacing(), 0.2f));
			}
		//		- second player
			if (input.wasKeyPressed(SDL_SCANCODE_G)) {
				this->_bullets.push_back(Bullet(graphics, Vector2((int)this->_players.at(1).getX(), (int)this->_players.at(1).getY()),
					this->_players.at(1).getFacing(), 0.2f));
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

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		this->update((float)(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME)));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
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

void Game::update(float elapsedTime) {

	// Updating players position
	for (int i = 0; i < (int)this->_players.size(); i++) {
		this->_players.at(i).update(elapsedTime);
	}

	// Updating the level (Animated level only)
	this->_level.update(elapsedTime);

	// Updating bullet position and destroy them if they're out of bound
	for (int i = 0; i < (int)this->_bullets.size(); i++) {
		this->_bullets.at(i).update(elapsedTime);
		if (this->_bullets.at(i).getX() > globals::SCREEN_WIDTH || this->_bullets.at(i).getX() < 0) {
			this->_bullets.erase(this->_bullets.begin() + i);
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
			this->_bullets.erase(this->_bullets.begin() + i);
		}
	}

	// Check projectile collisions with players

	// Check player collisions with slopes
	for (int i = 0; i < (int)this->_players.size(); i++) {
		std::vector<Slope> otherSlopes;
		if ((otherSlopes = this->_level.checkSlopeCollision(this->_players.at(i).getBoundingBox())).size() > 0) {
			this->_players.at(i).handleSlopeCollisions(otherSlopes);
		}
	}
}