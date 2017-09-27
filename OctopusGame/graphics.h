#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <map>

#include <SDL_image.h>

class GraphicalOctopus {
public:
	GraphicalOctopus();
	~GraphicalOctopus();

	// Loads an image into the _spriteSheets map if it doesn't already exist, so that each image will only
	// be loaded once. Returns the image in both cases.
	SDL_Surface* loadImage(const std::string &filePath);

	// Draws the texture to a certain part of the screen
	void blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);

	// Renders everything to the screen
	void flip();

	// Clears the screen
	void clear();
	
	// Returns the renderer
	SDL_Renderer* getRenderer() const;
private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;

	std::map<std::string, SDL_Surface*> _spriteSheets;
};

#endif