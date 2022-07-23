#include "Camera.h"
#include "Game.h"
#include "General.h"

Camera::Camera() : _x(0.f), _y(0.f), _grid(45), _fgrid(45.f) {

}

int Camera::convert_x(int x) const {
	return x - (int)_x;
}

int Camera::convert_y(int y) const {
	return y - (int)_y;
}

float Camera::convert_x(float x) const {
	return x - (float)_x;
}

float Camera::convert_y(float y) const
{
	return y - (float)_y;
}

int Camera::convert_x_to_j(float x) const
{
	return (int)floorf(x / _fgrid);
}

int Camera::convert_y_to_i(float y) const
{
	return (int)floorf(y / _fgrid);
}

void Camera::play_logic(Game& g)
{
	Player& p = g._entity_handler._p;
	float target_x = p.get_mid_x() - g._WIDTH / 2.f;
	float target_y = p.get_mid_y() - g._HEIGHT / 2.f;

	float speed = 0.2f;
	_x += (target_x - _x) * speed;
	_y += (target_y - _y) * speed;

	//if (_x < 0) _x = 0;
	//if (_y < 0) _y = 0;
}
void Camera::edit_logic(Game& g)
{

	// movement
	static const float speed = 0.65f;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_RIGHT]) {
		_x += speed * g._dt;
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		_x -= speed * g._dt;
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		_y += speed * g._dt;
	}
	if (keys[SDL_SCANCODE_UP]) {
		_y -= speed * g._dt;
	}

	// placing things
	if (g._mouse_btn_pressed_this_frame[0]) { // place block tile on left click
		g._tile_handler.place_tile(g, g._mouse_pos_on_latest_press[0], g._mouse_pos_on_latest_press[1]);
	}
	if (g._mouse_btn_pressed_this_frame[2]) { // place enemy on right click
		g._entity_handler.place_enemy(g, g._mouse_pos_on_latest_press[0], g._mouse_pos_on_latest_press[1]);
	}
}

void Camera::draw_grid(Game& g)
{
	// draw grid lines
	SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);

	const int x_offset = General::mod((int)_x, _grid);
	const int y_offset = General::mod((int)_y, _grid);
	const int x_tiles = g._WIDTH / _grid + 2;
	const int y_tiles = g._HEIGHT / _grid + 2;

	{
		int x = -x_offset;
		for (int i = 0; i < x_tiles; ++i) { // from top to bottom
			if (_x + x >= 0)
				SDL_RenderDrawLine(g._renderer, x, 0, x, g._HEIGHT);
			x += _grid;
		}
	}
	{
		int y = -y_offset;
		for (int i = 0; i < y_tiles; ++i) { // left to right
			if (_y + y >= 0)
				SDL_RenderDrawLine(g._renderer, 0, y, g._WIDTH, y);
			y += _grid;
		}
	}
}

void Camera::draw_hud(Game& g)
{
	Player& p = g._entity_handler._p;

	// hp of player
	{
		// red health bar
		SDL_SetRenderDrawColor(g._renderer, 255, 0, 0, 127);

		SDL_Rect draw_rect = { 25, 25, (int)(100.f * ((float)p._hp / p._max_hp)), 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);

		// black boundary
		draw_rect.w = 100;
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 127);
		SDL_RenderDrawRect(g._renderer, &draw_rect);
	}

	// coins of player
	{
		SDL_SetRenderDrawColor(g._renderer, 230, 230, 0, 127);

		SDL_Rect draw_rect = { 25, 50, (int)(10.f * ((float)p._coins / 1)), 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);
	}

	// shots player has
	{
		SDL_SetRenderDrawColor(g._renderer, 75, 75, 75, 127);

		SDL_Rect draw_rect = { 25, 75, 5 * p._shots, 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);
	}

	// bomb throw charge meter
	{
		// green charge (drawn first, BECAUSE: obvious)

		float max_x = 200.f;
		int charge_x = (int)(max_x * (p._bomb_throw_charge / p._bomb_throw_max_charge));
		SDL_Rect draw_rect = { 25, 100, charge_x, 20  };
		SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, 127);
		SDL_RenderFillRect(g._renderer, &draw_rect);

		// black boundary
		draw_rect.w = (int)max_x;
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 127);
		SDL_RenderDrawRect(g._renderer, &draw_rect);
	}
}