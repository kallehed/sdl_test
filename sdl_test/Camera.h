#pragma once
#include <string>

class Game;
struct SDL_Color;

enum class EDIT : char {
	BLOCK_TILE = 0,
	TRI_NE,
	TRI_SE,
	TRI_NW,
	TRI_SW,
	DESTRUCTABLE_TILE,
	ENEMY_BASIC,
	ENEMY_SHOOTER,
	TOTAL
};

class Camera {
public:
	float _x;
	float _y;
	const int _grid;
	const float _fgrid;

	EDIT _edit = EDIT::BLOCK_TILE;

	Camera();
	int convert_x(int x) const;
	int convert_y(int y) const;
	float convert_x(float x) const;
	float convert_y(float y) const;
	int convert_x_to_j(float x) const;
	int convert_y_to_i(float y) const;

	void play_logic(Game& g);

	void edit_logic(Game& g);

	void draw_edit(Game& g);
	void draw_text(Game& g, std::string& text, SDL_Color& color, int x, int y, int scale);
	void draw_edit_text(Game& g);
	void draw_grid(Game& g);

	void draw_play(Game& g);
	void draw_hud(Game& g);
};