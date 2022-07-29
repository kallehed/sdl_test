#pragma once
#include <array>

class Game;

// defines only shape and game logic, not look
enum class TILE : char {
	VOID = 0, // higher elements are not walkthrough, and are drawn
	BLOCK,
	DESTRUCTABLE,
	// TILES BELOW TRI_NE ARE COMPLETELY FULL
	TRI_NE, // triangle, hypothenuse towards north east
	TRI_SE, // south east
	TRI_NW, // north west
	TRI_SW, // ...
	TOTAL
};

class TileHandler {
public:
	static const int _len = 100;
	//TILE _tiles[_len][_len];
	std::array<std::array<TILE, _len>, _len> _tiles;

	TileHandler();
	void draw(Game& g);
	void place_tile(Game& g, TILE tile, int x, int y);

	// returns true if tile changed, otherwise false
	bool remove_tile(Game& g, int x, int y);
	void hurt_tile(int i, int j);

	template<TILE tile>
	static bool intersection_tile(float x, float y, float w, float h, float t_x, float t_y, float t_w, float t_h);

	bool tile_in_range(int i, int j) const;
	TILE get_tile_type(int i, int j);
	bool is_blocking_tile(int i, int j);
	bool is_path_clear(Game& g, float x1, float y1, float x2, float y2);

};

