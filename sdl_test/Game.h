#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <array>

#include "Camera.h"
#include "TileHandler.h"
#include "EntityHandler.h"

// DONT HAVE ENUMS/OTHER CLASSES THAN GAME/STRUCTS IN THIS FILE!!!!

class Game {
public:
	const int _HEIGHT = 600;
	const int _WIDTH = (int)(_HEIGHT * 1.77777777); // for screen
	int _scale = 1;

	long long _ticks = 0;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	TTF_Font* _font = nullptr;

	//textures
	SDL_Texture* _textures[TEX::TOTAL];

	float _dt = 0; // time for previous frame to run in MS
	const float _MAX_DT = 1000.f / 50.f; // 50 is the lowest framerate allowed. Also maximum _dt

	bool _edit_mode = false;

	std::array<bool, 3> _mouse_btn_pressed_this_frame = { false, false, false }; // left, middle, right
	std::array<int, 2> _mouse_pos_on_latest_press = {0, 0};
	int32_t _mouse_scroll = 0;

	// keys pressed THIS frame, access with [SDLK_x]
	std::array<bool, 256> _keys_frame;

	Camera _cam;

	TileHandler _tile_handler;

	EntityHandler _entity_handler;

	SDL_Texture* loadTexture(const char* path);

	void changedScale();
	Uint32 getMouseState(int* x, int* y);

	Game();
	void start_game();
	void close_game();
	void game_loop();
	void game_logic();

	template <bool EDIT>
	void game_draw();
};