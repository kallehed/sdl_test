#pragma once

class Game;

class TileHandler {
public:
	static const int _len = 100;
	bool _block_tiles[_len][_len];

	TileHandler();
	void draw(Game& g);
	void place_tile(Game& g, int x, int y);
	bool block_tile_in_range(int i, int j);
	bool is_block_tile(int i, int j);
	bool is_path_clear(Game& g, float x1, float y1, float x2, float y2);
};

