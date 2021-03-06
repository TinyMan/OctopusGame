// graphics.cpp
// File handling all the graphic components of the project

#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "globals.h"
#include <sstream>

// Constructor
GraphicalOctopus::GraphicalOctopus() {
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Octopus Game");
}

// Destructor
GraphicalOctopus::~GraphicalOctopus() {
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

// Loads an image into the _spriteSheets map if it doesn't already exist, so that each image will only
// be loaded once. Returns the image in both cases.
SDL_Texture* GraphicalOctopus::loadImage(const std::string &filePath) {
	if (this->_spriteSheets.count(filePath) == 0) {
		this->_spriteSheets[filePath] = SDL_CreateTextureFromSurface(this->getRenderer(), IMG_Load(filePath.c_str()));
	}
	return this->_spriteSheets[filePath];	
}

// Draws the texture to a certain part of the screen
void GraphicalOctopus::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

// Renders everything to the screen
void GraphicalOctopus::flip() {
	Uint32 t = time();
	Uint32 delta = t - _lastFpsUpdate;
	if (delta > _fpsUpdateInterval)
	{
		//LOGINFO << "Updating fps count" << endl;
		_fps = (float)_nbFrames / (delta / 1000.f);
		_nbFrames = 0;
		_lastFpsUpdate = t;
		std::stringstream s;
		s << "Octopus Game - " <<_fps << " fps";
		SDL_SetWindowTitle(_window, s.str().c_str());
	}
	SDL_RenderPresent(this->_renderer);
	_nbFrames++;
}

// Clears the screen
void GraphicalOctopus::clear() {
	_frameStart = SDL_GetTicks();
	SDL_RenderClear(this->_renderer);
}

// Returns the renderer
SDL_Renderer* GraphicalOctopus::getRenderer() const {
	return this->_renderer;
}
