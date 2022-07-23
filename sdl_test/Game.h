#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <array>

#include "Camera.h"
#include "TileHandler.h"
#include "EntityHandler.h"

class Game {
public:
	const int _WIDTH = 1067; // for screen
	const int _HEIGHT = 600;
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	TTF_Font* _font = nullptr;

	float _dt = 0; // time for previous frame to run in MS
	const float _MAX_DT = 1000.f / 45.f; // 45 is the lowest framerate allowed. Also maximum _dt

	bool _edit_mode = false;

	std::array<bool, 3> _mouse_btn_pressed_this_frame = { false, false, false }; // left, middle, right
	std::array<int, 2> _mouse_pos_on_latest_press = {0, 0};

	Camera _cam;

	TileHandler _tile_handler;

	EntityHandler _entity_handler;

	Game();
	void start_game();
	void close_game();
	void game_loop();
	void game_logic();
	void game_draw();
};