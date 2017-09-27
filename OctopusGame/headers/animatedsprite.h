#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <string>
#include <map>
#include <vector>
#include "sprite.h"
#include "globals.h"

class GraphicalOctopus;

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(GraphicalOctopus &graphics, const std::string filePath, int sourceX, int sourceY,
		int width, int height, float posX, float posY, float timeToUpdate);

	// Plays the animation by name if it is not already playing
	// Can be played once or looped by default
	void playAnimation(std::string animation, bool once = false);

	// Updates the animated sprite (timer)
	void update(float elapsedTime);

	// Draws the sprite on the screen
	void draw(GraphicalOctopus &graphics, int x, int y);


protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	// Adds an animation to the map of animations
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	// Resets all animations animated with this sprite
	void resetAnimations();

	// Stops the current animation
	void stopAnimation();

	// Changes the visibility of the animated sprite
	void setVisible(bool visible);

	// Logic that happens when an animation ends
	virtual void animationDone(std::string currentAnimation) = 0;

	// A required function that sets up all the animation for a sprite
	virtual void setupAnimations() = 0;

private:
	std::map<std::string, std::vector<SDL_Rect> > _animations;
	std::map<std::string, Vector2> _offsets;

	int _frameIndex;
	double _timeElapsed;
	bool _visible;
};

#endif // !ANIMATED_SPRITE_H
