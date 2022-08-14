#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <array>
#include <string>
#include <bitset>
#include <unordered_set>
#include <algorithm>

#include "Camera.h"
#include "TileHandler.h"
#include "EntityHandler.h"

// DONT HAVE ENUMS/OTHER CLASSES THAN GAME/STRUCTS IN THIS FILE!!!!

// Hash for std::array<int, 2>
struct HashArray2Int {
public:
	size_t operator()(std::array<int, 2> ar) const {
		int val = ar[0] * ar[1];
		return std::hash<int>()(val);
	}
};
// Custom comparator that compares the array objects
struct EqualArray2Int {
public:
	bool operator()(std::array<int, 2> ar1, std::array<int, 2> ar2) const {
		if (ar1 == ar2)
			return true;
		else
			return false;
	}
};

class Game {
public:
	static constexpr int _HEIGHT = 600;
	static constexpr int _WIDTH = (int)(_HEIGHT * 1.77777777); // for screen
	int _scale = 1;

	long long _ticks = 0;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	TTF_Font* _font = nullptr;

	//textures
	SDL_Texture* _textures[TEX::TOTAL];
	SDL_Texture* _press_e_texture;
	std::array<int, 2> _press_e_w_and_h; // width and height of "press e" texture

	float _dt = 0; // time for previous frame to run in MS
	const float _MAX_DT = 1000.f / 50.f; // 50 is the lowest framerate allowed. Also maximum _dt
	
	float _slow_motion_factor = 1.f;

	bool _edit_mode = false;

	std::array<bool, 3> _mouse_btn_pressed_this_frame = { false, false, false }; // left, middle, right
	std::array<int, 2> _mouse_pos_on_latest_press = {0, 0};
	int32_t _mouse_scroll = 0;

	// how many keys there are
	static constexpr int _KEY_BOOLS = 256;
	// keys CURRENTLY pressed
	std::array<bool, _KEY_BOOLS> _keys_down;

	// keys pressed THIS frame, access with [SDLK_x]
	std::array<bool, _KEY_BOOLS> _keys_frame;

	Camera _cam;

	TileHandler _tile_handler;

	EntityHandler _entity_handler;

	// what level we are on
	LEVEL::LEVEL _level = LEVEL::START;
	bool _change_level = false;
	std::string _destination_portal = "Error_Name";

	// onetime_index stuff
	// So the first number represents the level
	// The second number represents index in which something appears
	std::unordered_set<std::array<int, 2>, HashArray2Int, EqualArray2Int> _onetimes;

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