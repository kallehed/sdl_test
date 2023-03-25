#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <array>
#include <string>
#include <bitset>
#include <unordered_set>
#include <algorithm>

#include "Camera.h"
#include "TileHandler.h"
#include "EntityHandler.h"
#include "SaveState.h"

// DONT HAVE ENUMS/OTHER CLASSES THAN GAME/STRUCTS IN THIS FILE!!!!

// Draw circle
void draw_circle(SDL_Renderer*, float x, float y, float radius, SDL_Color);
// Vibrates the phone, ONLY if on a phone
void vibrate_phone(int millis);

class Game {
public:
	static constexpr int _HEIGHT = 600;
	static constexpr int _WIDTH = (int)(_HEIGHT * 1.777777777777); // for screen

	// for android, should be in lower left corner of screen
	static constexpr int _MOVEBOX_LEN = 200;
	// divided into a 3x3 box for movement on ANDROID
	static constexpr float _MOVEBOX_PART = _MOVEBOX_LEN/3.f;

	long long _ticks = 0;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	TTF_Font* _font = nullptr;

	// DEV::DEV is in "enums.h" (gets you abilites direct)

	// music
	Mix_Music* _music[MUS::TOTAL];
	MUS::_ _current_music = (MUS::_)(-1);

	// sound effects
	Mix_Chunk* _sound[SOUND::TOTAL];

	//textures
	SDL_Texture* _textures[TEX::TOTAL];
	SDL_Texture* _press_e_texture;
	std::array<int, 2> _press_e_w_and_h; // width and height of "press e" texture

	float _dt = 0; // time for previous frame to run in MS
	const float _MAX_DT = 1000.f / 60.f; // 50 is the lowest framerate allowed. Also maximum _dt
	
	float _slow_motion_factor = 1.f;

	bool _edit_mode = false;

	std::array<bool, 3> _mouse_btn_pressed_this_frame = { false, false, false }; // left, middle, right
	std::array<bool, 3> _mouse_btn_down = {false, false, false}; // same as prev line
	std::array<int, 2> _mouse_pos = {0, 0};
	int32_t _mouse_scroll = 0;

	// how many keys there are
	static constexpr int _KEY_BOOLS = 256;
	// keys CURRENTLY pressed
	std::array<bool, _KEY_BOOLS> _keys_down;

	// keys pressed THIS frame, access with [SDL_SCANCODE_X]
	std::array<bool, _KEY_BOOLS> _keys_frame;

	Camera _cam;

	TileHandler _tile_handler;

	EntityHandler _entity_handler;

	SaveState _save;

	// what level we are on
	int _level = DEV::DEV ? 0 : 0;
	int _level_to_change_to = 1;
	bool _change_level = false;
	std::string _destination_portal = "Error_Name";

	SDL_Texture* loadTexture(const char* path);

	void play_music(MUS::_ music);
	void play_sound(SOUND::_ sound);

	void changeScale(int change);

	Game();
	void start_game();
	void close_game();
	void game_loop();
	void game_logic();

	template <bool EDIT>
	void game_draw();
};