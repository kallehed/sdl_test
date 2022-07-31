#pragma once
#include <array>

// defines only shape and game logic AND look
// TYPES are specified by FULL CAPITAL,
// inbetween are look specific
class Game;

namespace TILE {
	enum TILE : int8_t {
		VOID = 0, // higher elements are not walkthrough, and are drawn
		BLOCK,
		DESTRUCTABLE,

		TRI_NE, // triangle, hypothenuse towards north east
		TRI_SE, // south east,
		TRI_NW, // north west,
		TRI_SW, // ...

		TOTAL
	};
}

namespace TEX {
	enum TEX : int8_t {
		VOID = -1,
		FireMagic = 0,
		BombExplosion,
		Bullet,
		BlueSlime,
		GreenSlime1,
		GreenSlime2,
		RedHuman,
		Coin,
		Container,

		Bush, // Beginning of tile images is at TEX::Bush
		Bush2,
		TreeStump,
		SmallTree1,
		SmallTree2,
		TOTAL
	};
}

class TileHandler {
public:
	static constexpr int _len = 50;
	// _tiles represents functionality
	std::array<std::array<TILE::TILE, _len>, _len> _tiles;
	std::array<std::array<TEX::TEX, _len>, _len> _texs;

	void TileHandler_construct(Game& g);
	void draw(Game& g);
	void place_tile(Game& g, TILE::TILE tile, int x, int y);

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

};

