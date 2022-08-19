#pragma once
#include "TileHandler.h"
#include "Button.h"
#include "enums.h"
#include <string>

class Game;
struct SDL_Color;

class Camera {
public:
	float _x = 0.f;
	float _y = 0.f;
	const int _grid = 55;
	const float _fgrid = (float)_grid;

	EDIT_MODE::EDIT_MODE _edit_mode = EDIT_MODE::TILE;

	TILE::TILE _edit_tile = TILE::BLOCK;
	TEX::TEX _edit_tex = TEX::Bush;
	ENTITIES::ENTITIES _edit_entity = ENTITIES::ENEMY_BASIC;

	std::array<Button, CAM_BTN::TOTAL> _btns;

	CAM_VIEW::CAM_VIEW _cam_view = CAM_VIEW::TEX;

	// shaking
	float _shake_divider = 1.5f; // shake intensity will be divided by _shake_divider every frame
	float _shake_intensity = 1.f;

	float _max_x;
	float _max_y;

	void construct(Game& g);

	void shake(Game& g, float divider, float intensity);

	int convert_x(int x) const;
	int convert_y(int y) const;
	float convert_x(float x) const;
	float convert_y(float y) const;
	int convert_x_to_j(float x);
	int convert_y_to_i(float y);

	void play_logic(Game& g);

	void edit_logic(Game& g);

	void save_to_file(Game& g);

	void load_from_file(Game& g, int level);

	void draw_edit(Game& g);
	void draw_text(Game& g, const char* text, const SDL_Color& color, int x, int y, int scale);
	void draw_text_background(Game& g, const char* text, const SDL_Color& color, const SDL_Color& color2, int x, int y, int scale);
	void draw_edit_text(Game& g);
	void draw_grid(Game& g);

	void draw_play(Game& g);
	void draw_hud(Game& g);
};