#include "TileHandler.h"
#include "Game.h"
#include "General.h"

#include <cassert>  

TileHandler::TileHandler()
{
	for (int i = 0; i < _len; ++i) {
		for (int j = 0; j < _len; ++j) {
			_tiles[i][j] = TILE::VOID;
		}
	}
}

void TileHandler::draw(Game& g)
{	
	float x = g._cam._x;
	float y = g._cam._y;
	float w = (float)g._WIDTH;
	float h = (float)g._HEIGHT;
	int j_start = g._cam.convert_x_to_j(x);
	int j_end = g._cam.convert_x_to_j((x + w - 0.01f)) + 1;
	int i_start = g._cam.convert_y_to_i(y);
	int i_end = g._cam.convert_y_to_i((y + h - 0.01f)) + 1;

	float r_w = g._cam._fgrid;
	float r_h = g._cam._fgrid;
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE tile = this->get_tile_type(i, j);
			if (tile > TILE::VOID) {
				int x = g._cam.convert_x(g._cam._grid * j);
				int y = g._cam.convert_y(g._cam._grid * i);
				if (i < 0 || j < 0) {
					SDL_Rect rect = { x, y, g._cam._grid, g._cam._grid };
					SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 75);
					SDL_RenderFillRect(g._renderer, &rect);
				} else 
				if (tile == TILE::BLOCK)
				{
					SDL_Rect rect = { x, y, g._cam._grid, g._cam._grid };
					SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(g._renderer, &rect);
				}
				else if (tile == TILE::DESTRUCTABLE)
				{
					SDL_Rect rect = { x, y, g._cam._grid, g._cam._grid };
					SDL_SetRenderDrawColor(g._renderer, 25, 150, 25, 255);
					SDL_RenderFillRect(g._renderer, &rect);
				}
				else if (tile == TILE::TRI_NE) {
					SDL_Color color = { 0,0,0,255 };
					SDL_Vertex vert[3];
					float fx = (float)x;
					float fy = (float)y;
					vert[0].position = { fx, fy };
					vert[0].color = color;
					vert[1].position = { fx , fy + g._cam._fgrid };
					vert[1].color = color;
					vert[2].position = { fx + g._cam._fgrid, fy + g._cam._fgrid };
					vert[2].color = color;
					SDL_RenderGeometry(g._renderer, NULL, vert, 3, NULL, 0);
				}
				else if (tile == TILE::TRI_SE) {
					SDL_Color color = { 0,0,0,255 };
					SDL_Vertex vert[3];
					float fx = (float)x;
					float fy = (float)y;
					vert[0].position = { fx, fy };
					vert[0].color = color;
					vert[1].position = { fx , fy + g._cam._fgrid };
					vert[1].color = color;
					vert[2].position = { fx + g._cam._fgrid, fy };
					vert[2].color = color;
					SDL_RenderGeometry(g._renderer, NULL, vert, 3, NULL, 0);
				}
				else if (tile == TILE::TRI_NW) {
					SDL_Color color = { 0,0,0,255 };
					SDL_Vertex vert[3];
					float fx = (float)x;
					float fy = (float)y;
					vert[0].position = { fx + g._cam._fgrid, fy + g._cam._fgrid };
					vert[0].color = color;
					vert[1].position = { fx , fy + g._cam._fgrid };
					vert[1].color = color;
					vert[2].position = { fx + g._cam._fgrid, fy };
					vert[2].color = color;
					SDL_RenderGeometry(g._renderer, NULL, vert, 3, NULL, 0);
				}
				else if (tile == TILE::TRI_SW) {
					SDL_Color color = { 0,0,0,255 };
					SDL_Vertex vert[3];
					float fx = (float)x;
					float fy = (float)y;
					vert[0].position = { fx + g._cam._fgrid, fy + g._cam._fgrid };
					vert[0].color = color;
					vert[1].position = { fx , fy };
					vert[1].color = color;
					vert[2].position = { fx + g._cam._fgrid, fy };
					vert[2].color = color;
					SDL_RenderGeometry(g._renderer, NULL, vert, 3, NULL, 0);
				}
			}
		}
	}
}

void TileHandler::place_tile(Game& g, TILE tile, int x, int y)
{
	assert(tile != TILE::TOTAL);

	int cam_x = (int)g._cam._x;
	int cam_y = (int)g._cam._y;
	int i = (y + cam_y - General::mod(y + cam_y, g._cam._grid)) / g._cam._grid;
	int j = (x + cam_x - General::mod(x + cam_x, g._cam._grid)) / g._cam._grid;
	if (tile_in_range(i, j))
	{
		_tiles[i][j] = tile;
	}
}
bool TileHandler::remove_tile(Game& g, int x, int y) {
	int cam_x = (int)g._cam._x;
	int cam_y = (int)g._cam._y;
	int i = (y + cam_y - General::mod(y + cam_y, g._cam._grid)) / g._cam._grid;
	int j = (x + cam_x - General::mod(x + cam_x, g._cam._grid)) / g._cam._grid;
	if (tile_in_range(i, j)) {
		if (_tiles[i][j] != TILE::VOID)
		{
			_tiles[i][j] = TILE::VOID;
			return true;
		} 
	}
	return false;
}
void TileHandler::hurt_tile(int i, int j)
{
	// can't get "tile" beyond _tiles, even though they are "BLOCK"
	if (this->tile_in_range(i, j))
	{ 
		TILE& tile = _tiles[i][j];
		if (tile == TILE::DESTRUCTABLE)
		{
			tile = TILE::VOID;
		}
	}
}

bool TileHandler::tile_in_range(int i, int j) const
{
	return i >= 0 && j >= 0 && i < _len && j < _len;
}

TILE TileHandler::get_tile_type(int i, int j) {
	if (tile_in_range(i, j)) {
		return _tiles[i][j];
	}
	return TILE::BLOCK;
}
TILE TileHandler::get_tile_type_reverse(int i, int j) {
	return this->get_tile_type(j, i);
}

bool TileHandler::is_blocking_tile(int i, int j)
{
	if (tile_in_range(i, j)) {
		return _tiles[i][j] > TILE::VOID;
	}
	else {
		return true;
	}
}

bool TileHandler::is_path_clear(Game& g, float x1, float y1, float x2, float y2)
{
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

		auto in_block_tile = General::get_blocking_tile_pos_in_area(g, x, y, tile_size, tile_size);
		if (std::get<0>(in_block_tile)) {
			return false;
		}

	}
	return true;
}
