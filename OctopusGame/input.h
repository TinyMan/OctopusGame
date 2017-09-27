#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <map>

class Input {
public:
	// Gets called at the beginning of each new frame to reset the keys that are no longer relevant
	void beginNewFrame();
	// Gets called when a key is being pressed
	void keyUpEvent(const SDL_Event &event);

	// Gets called when a key is released
	void keyDownEvent(const SDL_Event &event);

	// Checks if a certain key is currently being held
	bool isKeyHeld(SDL_Scancode key);

	// Checks if a certain key was pressed during the current frame
	bool wasKeyPressed(SDL_Scancode key);

	// Checks if a certain key was released during the current frame	
	bool wasKeyReleased(SDL_Scancode key);

private:
	std::map<SDL_Scancode, bool> _heldKeys;
	std::map<SDL_Scancode, bool> _pressedKeys;
	std::map<SDL_Scancode, bool> _releasedKeys;
};

#endif // !INPUT_H
