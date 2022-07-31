#pragma once

#include <SDL.h>

class Game;

namespace BTN {
	enum BTN : int8_t {
		NOTHING = 0,
		HELD_OVER,
		CLICKED_ON,
		TOTAL
	};
}

class Button
{
public:
	SDL_Texture* _text_texture;
	SDL_Rect _rect;
	bool _mouse_held_over = false;
	bool _clicked_on = false;
	void construct(Game& g, int x, int y, const char* text);
	~Button();

	// returns true if mouse OVER button
	BTN::BTN logic(Game& g);
	void draw(Game& g);

};

