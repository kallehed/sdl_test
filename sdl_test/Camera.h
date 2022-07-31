#pragma once
#include <string>
#include "TileHandler.h"
#include "Button.h"

class Game;
struct SDL_Color;

//namespace EDIT {
//	enum EDIT : int8_t {
//		BLOCK_TILE = 0,
//		DESTRUCTABLE_TILE,
//		TRI_NE,
//		TRI_SE,
//		TRI_NW,
//		TRI_SW,
//		ENEMY_BASIC,
//		ENEMY_SHOOTER,
//		TOTAL
//	};
//}
namespace EDIT_MODE {
	enum EDIT_MODE : int8_t {
		TILE,
		TEX,
		ENTITY,
		TOTAL
	};
}

namespace ENTITIES {
	enum ENTITIES : int8_t {
		ENEMY_BASIC,
		ENEMY_SHOOTER,
		TOTAL
	};
}

class Camera {
public:
	float _x = 0.f;
	float _y = 0.f;
	const int _grid = 50;
	const float _fgrid = (float)_grid;


	EDIT_MODE::EDIT_MODE _edit_mode = EDIT_MODE::TILE;

	TILE::TILE _edit_tile = TILE::BLOCK;
	TEX::TEX _edit_tex = TEX::Bush;
	ENTITIES::ENTITIES _edit_entity = ENTITIES::ENEMY_BASIC;

	Button _save_btn;
	Button _load_btn;

	void construct(Game& g);

	int convert_x(int x) const;
	int convert_y(int y) const;
	float convert_x(float x) const;
	float convert_y(float y) const;
	int convert_x_to_j(float x);
	int convert_y_to_i(float y);

	void play_logic(Game& g);

	void edit_logic(Game& g);

	void draw_edit(Game& g);
	void draw_text(Game& g, const char* text, const SDL_Color& color, int x, int y, int scale);
	void draw_edit_text(Game& g);
	void draw_grid(Game& g);

	void draw_play(Game& g);
	void draw_hud(Game& g);
};