#include "TileHandler.h"
#include "Game.h"
#include "General.h"

TileHandler::TileHandler() {
	for (int i = 0; i < _len; ++i) {
		for (int j = 0; j < _len; ++j) {
			_block_tiles[i][j] = false;
		}
	}
}

void TileHandler::draw(Game& g) {
	SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
	for (int i = 0; i < _len; ++i) {
		for (int j = 0; j < _len; ++j) {
			if (_block_tiles[i][j]) {
				int x = g._cam._grid * j;
				int y = g._cam._grid * i;
				SDL_Rect rect = { g._cam.convert_x(x), g._cam.convert_y(y), g._cam._grid, g._cam._grid };
				SDL_RenderFillRect(g._renderer, &rect);
			}
		}
	}
}

void TileHandler::place_tile(Game& g, int x, int y) {
	int cam_x = (int)g._cam._x;
	int cam_y = (int)g._cam._y;
	int i = (y + cam_y - General::mod(y + cam_y, g._cam._grid)) / g._cam._grid;
	int j = (x + cam_x - General::mod(x + cam_x, g._cam._grid)) / g._cam._grid;
	if (block_tile_in_range(i, j)) {
		_block_tiles[i][j] = true;
	}
}

bool TileHandler::block_tile_in_range(int i, int j)
{
	return i >= 0 && j >= 0 && i < _len && j < _len;
}

bool TileHandler::is_block_tile(int i, int j)
{
	if (block_tile_in_range(i, j)) {
		return _block_tiles[i][j];
	}
	else {
		return true;
	}
}

bool TileHandler::is_path_clear(Game& g, float x1, float y1, float x2, float y2) {

	// go from start to end in diagonal, check for blocking tiles
	float nx, ny; // normalized vector and length
	float length = General::normalize_vector_two_points(nx, ny, x1, y1, x2, y2);

	if (length == 0.f) { // if not: will trigger infinite loop
		return true;
	}

	float step = g._cam._fgrid/2.f;
	int steps = (int)(length / step) - 1;
	float x = x1, y = y1;
	float tile_size = (float)g._cam._grid;

	for (int index = 0; index < steps; ++index) {
		x += nx * step;
		y += ny * step;

		std::pair<bool, std::array<int, 2>> in_block_tile = General::get_blocking_tile_pos_in_area(g, x, y, tile_size, tile_size);
		if (in_block_tile.first) {
			return false;
		}

	}
	return true;
}
