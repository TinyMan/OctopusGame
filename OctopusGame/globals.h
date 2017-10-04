#ifndef GLOBAL_H
#define GLOBAL_H

namespace globals {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	const int TILE_SCALE = 2;
	const int SPRITE_SCALE = 1;

	const float FORCE_REDUCTION = 2.0f / 3.0f;
}

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	inline Side getOppositeSide(Side side) {
		return 
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Vector2 {
	int x;
	int y;

	Vector2() :
		x(0), y(0) {}
	Vector2(int x, int y) :
		x(x), y(y) {}
};

struct Vector2f {
	float x;
	float y;

	Vector2f() :
		x(0), y(0) {}
	Vector2f(float x, float y) :
		x(x), y(y) {}
};

struct Force {
	Vector2f direction;
	float power;
	int timeLeft;

	Force() :
	power(0), direction(Vector2f(0.0f, 0.0f)), timeLeft(0) {}

	Force(float power, int timeLeft) :
		power(power), timeLeft(timeLeft) 
	{
		direction = Vector2f(0.0f, 0.0f);
	}
};

#endif // !GLOBAL_H
