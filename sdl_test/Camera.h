#pragma once

class Game;

class Camera {
public:
	float _x;
	float _y;
	const int _grid;
	const float _fgrid;

	Camera();
	int convert_x(int x) const;
	int convert_y(int y) const;
	float convert_x(float x) const;
	float convert_y(float y) const;
	int convert_x_to_j(float x) const;
	int convert_y_to_i(float y) const;
	void play_logic(Game& g);
	void edit_logic(Game& g);
	void draw_grid(Game& g);
	void draw_hud(Game& g);
};