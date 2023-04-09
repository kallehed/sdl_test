#pragma once
#include <array>
#include "enums.h"

class Game;

class TileHandler {
public:
	// how big the levels can be
	static constexpr int _len = 100; 
	// _tiles represents functionality
	std::array<std::array<TILE::TILE, _len>, _len> _tiles;
	std::array<std::array<TEX::TEX, _len>, _len> _texs;

	TEX::TEX _background_tile = TEX::SquareGreen;

	void TileHandler_construct(Game& g);

	void reset_all(Game& g);

	void draw_textures(Game& g, int alpha);

	void draw_shapes(Game& g);
	void draw(Game& g);
	void place_tile(Game& g, TILE::TILE tile, int x, int y);
	void place_tex(Game& g, TEX::TEX tile, int x, int y);

	// returns true if tile changed, otherwise false
	bool remove_tile(Game& g, int x, int y);
	bool hurt_tile(Game& g, int i, int j);

	template<TILE::TILE tile>
	static bool intersection_tile(float x, float y, float w, float h, float t_x, float t_y, float t_w, float t_h);

	bool tile_in_range(int i, int j) const;
	TILE::TILE get_tile_type(int i, int j);
	TEX::TEX get_tile_tex(int i, int j);
	bool is_blocking_tile(int i, int j);
	bool is_path_clear(Game& g, float x1, float y1, float x2, float y2);

	// returns true if any blocking tiles, then gives pos(x, y), then (i, j), then TILE type, of tile
	std::tuple<bool, std::array<int, 4>, TILE::TILE> get_blocking_tile_pos_in_area(Game& g, float x, float y, float w, float h);

};

