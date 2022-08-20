#include "TileHandler.h"
#include "Game.h"
#include "General.h"
#include "Pickupable.h"
#include "Particle.h"

#include <cassert>  

void TileHandler::TileHandler_construct(Game& g)
{
	reset_all(g);
}

void TileHandler::reset_all(Game& g) {
	// Functionality of tiles
	for (int i = 0; i < _len; ++i) {
		for (int j = 0; j < _len; ++j) {
			_tiles[i][j] = TILE::VOID;
			_texs[i][j] = TEX::VOID;
		}
	}
}

void TileHandler::draw_textures(Game& g, int alpha)
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
			TEX::TEX tex = this->get_tile_tex(i, j);
			int r_x = g._cam.convert_x(g._cam._grid * j);
			int r_y = g._cam.convert_y(g._cam._grid * i);
			SDL_Rect rect = { r_x, r_y, g._cam._grid, g._cam._grid };
			SDL_SetTextureAlphaMod(g._textures[_background_tile],alpha);
			SDL_RenderCopy(g._renderer, g._textures[_background_tile], NULL, &rect);
			
			TILE::TILE tile = this->get_tile_type(i, j);
			if (tex > TEX::VOID) {
				int r_x = g._cam.convert_x(g._cam._grid * j);
				int r_y = g._cam.convert_y(g._cam._grid * i);
				if (tile < TILE::TRI_NE) {

					SDL_Rect rect = { r_x, r_y, g._cam._grid, g._cam._grid };
					SDL_SetTextureAlphaMod(g._textures[tex], alpha);
					SDL_RenderCopy(g._renderer, g._textures[tex], NULL, &rect);
				}
				else {
					SDL_Color color = { 255, 255, 255, (Uint8)alpha };
					if (tile == TILE::TRI_NE) {
						SDL_Vertex vert[3];
						float fx = (float)r_x;
						float fy = (float)r_y;
						vert[0].position = { fx, fy };
						vert[0].color = color;
						vert[0].tex_coord = { 0.f,0.f };
						vert[1].position = { fx , fy + g._cam._fgrid };
						vert[1].color = color;
						vert[1].tex_coord = { 0.f, 1.f };
						vert[2].position = { fx + g._cam._fgrid, fy + g._cam._fgrid };
						vert[2].color = color;
						vert[2].tex_coord = { 1.f, 1.f };
						SDL_RenderGeometry(g._renderer, g._textures[tex], vert, 3, NULL, 0);
					}
					else if (tile == TILE::TRI_SE) {
						SDL_Vertex vert[3];
						float fx = (float)r_x;
						float fy = (float)r_y;
						vert[0].position = { fx, fy };
						vert[0].color = color;
						vert[0].tex_coord = { 0.f,0.f };
						vert[1].position = { fx , fy + g._cam._fgrid };
						vert[1].color = color;
						vert[1].tex_coord = { 0.f, 1.f };
						vert[2].position = { fx + g._cam._fgrid, fy };
						vert[2].color = color;
						vert[2].tex_coord = { 1.f, 0.f };
						SDL_RenderGeometry(g._renderer, g._textures[tex], vert, 3, NULL, 0);
					}
					else if (tile == TILE::TRI_NW) {
						SDL_Vertex vert[3];
						float fx = (float)r_x;
						float fy = (float)r_y;
						vert[0].position = { fx + g._cam._fgrid, fy + g._cam._fgrid };
						vert[0].color = color;
						vert[0].tex_coord = { 1.f,1.f };
						vert[1].position = { fx , fy + g._cam._fgrid };
						vert[1].color = color;
						vert[1].tex_coord = { 0.f, 1.f };
						vert[2].position = { fx + g._cam._fgrid, fy };
						vert[2].color = color;
						vert[2].tex_coord = { 1.f, 0.f };
						SDL_RenderGeometry(g._renderer, g._textures[tex], vert, 3, NULL, 0);
					}
					else if (tile == TILE::TRI_SW) {
						SDL_Vertex vert[3];
						float fx = (float)r_x;
						float fy = (float)r_y;
						vert[0].position = { fx + g._cam._fgrid, fy + g._cam._fgrid };
						vert[0].color = color;
						vert[0].tex_coord = { 1.f,1.f };
						vert[1].position = { fx , fy };
						vert[1].color = color;
						vert[1].tex_coord = { 0.f, 0.f };
						vert[2].position = { fx + g._cam._fgrid, fy };
						vert[2].color = color;
						vert[2].tex_coord = { 1.f, 0.f };
						SDL_RenderGeometry(g._renderer, g._textures[tex], vert, 3, NULL, 0);
					}
				}
			}
		}
	}
}

void TileHandler::draw_shapes(Game& g) {
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
			TILE::TILE tile = this->get_tile_type(i, j);
			int r_x = g._cam.convert_x(g._cam._grid * j);
			int r_y = g._cam.convert_y(g._cam._grid * i);
			if (tile == TILE::BLOCK)
			{
				SDL_Rect rect = { r_x, r_y, g._cam._grid, g._cam._grid };
				SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(g._renderer, &rect);
			}
			else if (tile == TILE::DESTRUCTABLE)
			{
				SDL_Rect rect = { r_x, r_y, g._cam._grid, g._cam._grid };
				SDL_SetRenderDrawColor(g._renderer, 25, 150, 25, 255);
				SDL_RenderFillRect(g._renderer, &rect);
			}
			else if (tile == TILE::TRI_NE) {
				SDL_Color color = { 0,0,0,255 };
				SDL_Vertex vert[3];
				float fx = (float)r_x;
				float fy = (float)r_y;
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
				float fx = (float)r_x;
				float fy = (float)r_y;
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
				float fx = (float)r_x;
				float fy = (float)r_y;
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
				float fx = (float)r_x;
				float fy = (float)r_y;
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

void TileHandler::draw(Game& g)
{
	if (g._cam._cam_view == CAM_VIEW::SHAPE) {
		draw_shapes(g);
	}
	else if (g._cam._cam_view == CAM_VIEW::TEX) {
		draw_textures(g, 255);
	}
	else if (g._cam._cam_view == CAM_VIEW::HALF) {
		draw_shapes(g);
		draw_textures(g, 127);
	}
}

// TAKES MOUSE POSITION RELATIVE TO SCREEN FOR SOME REASON
void TileHandler::place_tile(Game& g, TILE::TILE tile, int x, int y)
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

// TAKES MOUSE POSITION RELATIVE TO SCREEN FOR SOME REASON
void TileHandler::place_tex(Game& g, TEX::TEX tex, int x, int y)
{
	assert(tex != TEX::TOTAL);

	int cam_x = (int)g._cam._x;
	int cam_y = (int)g._cam._y;
	int i = (y + cam_y - General::mod(y + cam_y, g._cam._grid)) / g._cam._grid;
	int j = (x + cam_x - General::mod(x + cam_x, g._cam._grid)) / g._cam._grid;
	if (tile_in_range(i, j))
	{
		_texs[i][j] = tex;
	}
	else {
		_background_tile = tex;
	}
}

bool TileHandler::remove_tile(Game& g, int x, int y) {
	int cam_x = (int)g._cam._x;
	int cam_y = (int)g._cam._y;
	int i = (y + cam_y - General::mod(y + cam_y, g._cam._grid)) / g._cam._grid;
	int j = (x + cam_x - General::mod(x + cam_x, g._cam._grid)) / g._cam._grid;
	if (tile_in_range(i, j) && (_tiles[i][j] != TILE::VOID || _texs[i][j] != TEX::VOID)) {
		_tiles[i][j] = TILE::VOID;
		_texs[i][j] = TEX::VOID;
		return true;
	}
	return false;
}
// return true if tile was hurt
bool TileHandler::hurt_tile(Game& g, int i, int j)
{
	// can't get "tile" beyond _tiles, even though they are "BLOCK"
	if (this->tile_in_range(i, j))
	{
		TILE::TILE& tile = _tiles[i][j];
		if (tile == TILE::DESTRUCTABLE) // HURT TILE
		{
			tile = TILE::VOID;
			_texs[i][j] = TEX::VOID;

			// shake screen
			g._cam.shake(g, 1.5f, 5.f);

			// possibly drop items?
			{

				float rand = General::randf01();
				if (rand >= 0.4f) {
					float x = ((float)j + 0.5f)* g._cam._fgrid;
					float y = ((float)i + 0.5f) * g._cam._fgrid;
					float x_vel = (General::randf01() - 0.5f) / 15.f;
					float y_vel = (General::randf01() - 0.5f) / 15.f;
					if (rand <= 0.8f) {
						// drop heart
						Pickupable* coin = new Pickupable(PICKUPABLE_TYPE::HEART, x, y, x_vel, y_vel);
						g._entity_handler._entities_to_add.push_back(coin);
					}
					else if (rand <= 0.9f) {
						// drop pickupable shot
						Pickupable* shot = new Pickupable(PICKUPABLE_TYPE::SHOT, x, y, x_vel, y_vel);
						g._entity_handler._entities_to_add.push_back(shot);
					}
					else {
						// drop pickupable bomb
						Pickupable* e = new Pickupable(PICKUPABLE_TYPE::BOMB, x, y, x_vel, y_vel);
						g._entity_handler._entities_to_add.push_back(e);
					}
				}
			}

			// particles?
			{
				int total_particles = 14 + rand() % 14;
				float x = ((float)j + 0.5f) * g._cam._fgrid;
				float y = ((float)i + 0.5f) * g._cam._fgrid;
				
				for (int i = 0; i < total_particles; ++i) {
					float x_vel = (General::randf01() - 0.5f);
					float y_vel = (General::randf01() - 0.5f);
					Particle* e = new Particle(x, y, x_vel, y_vel, { 0,230,0,230 });
					g._entity_handler._particles.emplace_back(e);
				}

			}
			return true;
		}
	}
	return false;
}

template <TILE::TILE tile>
bool TileHandler::intersection_tile(float x, float y, float w, float h, float t_x, float t_y, float t_w, float t_h) {
	if constexpr (tile == TILE::TRI_NE) {
		return t_y + x - t_x <= y + h;
	}
	else if constexpr (tile == TILE::TRI_SE) {
		return x <= t_x + t_w - y + t_y;
	}
	else if constexpr (tile == TILE::TRI_NW) {
		return x + w >= t_x + t_y + t_h - y - h;
	}
	else if constexpr (tile == TILE::TRI_SW) {
		return x + w >= t_x + y - t_y;
	}
}

template bool TileHandler::intersection_tile<TILE::TRI_NE>(float x, float y, float w, float h, float t_x, float t_y, float t_w, float t_h);
template bool TileHandler::intersection_tile<TILE::TRI_SE>(float x, float y, float w, float h, float t_x, float t_y, float t_w, float t_h);
template bool TileHandler::intersection_tile<TILE::TRI_NW>(float x, float y, float w, float h, float t_x, float t_y, float t_w, float t_h);
template bool TileHandler::intersection_tile<TILE::TRI_SW>(float x, float y, float w, float h, float t_x, float t_y, float t_w, float t_h);

bool TileHandler::tile_in_range(int i, int j) const
{
	return i >= 0 && j >= 0 && i < _len && j < _len;
}

TILE::TILE TileHandler::get_tile_type(int i, int j) {
	if (tile_in_range(i, j)) {
		return _tiles[i][j];
	}
	return TILE::BLOCK;
}

TEX::TEX TileHandler::get_tile_tex(int i, int j) {
	if (tile_in_range(i, j)) {
		return _texs[i][j];
	}
	return TEX::SmallTree1;
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

		auto in_block_tile = get_blocking_tile_pos_in_area(g, x, y, tile_size, tile_size);
		if (std::get<0>(in_block_tile)) {
			return false;
		}

	}
	return true;
}

// returns true if any blocking tiles, then gives pos(x, y) of that tile, then (i, j) of tile
std::tuple<bool, std::array<int, 4>, char> TileHandler::get_blocking_tile_pos_in_area(Game& g, float x, float y, float w, float h)
{
	int j_start = g._cam.convert_x_to_j(x);
	int j_end = g._cam.convert_x_to_j((x + w - 0.01f)) + 1;
	int i_start = g._cam.convert_y_to_i(y);
	int i_end = g._cam.convert_y_to_i((y + h - 0.01f)) + 1;

	int r_w = g._cam._grid;
	int r_h = g._cam._grid;
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE::TILE tile = get_tile_type(i, j);
			if (tile > TILE::VOID) {
				if (tile < TILE::TRI_NE) {
				GOTO_RETURN:
					return { true, {j * r_w, i * r_h, i, j}, tile };
				}
				else if (tile == TILE::TRI_NE)
				{
					if (intersection_tile<TILE::TRI_NE>(x, y, w, h, j * r_w, i * r_h, r_w, r_h))
					{
						goto GOTO_RETURN;
					}
				}
				else if (tile == TILE::TRI_SE) {
					if (intersection_tile<TILE::TRI_SE>(x, y, w, h, j * r_w, i * r_h, r_w, r_h))
					{
						goto GOTO_RETURN;
					}
				}
				else if (tile == TILE::TRI_NW) {
					if (intersection_tile<TILE::TRI_NW>(x, y, w, h, j * r_w, i * r_h, r_w, r_h))
					{
						goto GOTO_RETURN;
					}
				}
				else if (tile == TILE::TRI_SW) {
					if (intersection_tile<TILE::TRI_SW>(x, y, w, h, j * r_w, i * r_h, r_w, r_h))
					{
						goto GOTO_RETURN;
					}
				}
			}
		}
	}
	return { false, {0,0,0,0}, TILE::VOID };
}