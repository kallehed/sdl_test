#include "Camera.h"
#include "Game.h"
#include "General.h"
#include "EnemyBasic.h"
#include "EnemyShooter.h"


Camera::Camera() : _x(0.f), _y(0.f) {

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

int Camera::convert_x_to_j(float x)
{
	return (int)floorf(x / _fgrid);
}

int Camera::convert_y_to_i(float y)
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

template <typename T>
void scroll_enum(Game& g, T& e, T total) {
	// scroll _edit
	e = (T)(e + g._mouse_scroll);
	e = (T)(General::mod((int)e, total));
}

void Camera::edit_logic(Game& g)
{

	// movement
	constexpr float speed = 0.65f;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
		_x += speed * g._dt;
	}
	if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
		_x -= speed * g._dt;
	}
	if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
		_y += speed * g._dt;
	}
	if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
		_y -= speed * g._dt;
	}

	// placing/removing things by clicking
	{
		// change _edit_mode
		if (keys[SDL_SCANCODE_L]) {
			_edit_mode = (EDIT_MODE::EDIT_MODE)(General::mod(_edit_mode + 1, EDIT_MODE::TOTAL ));
		}

		// mouse x and y, on screen
		int m_x;
		int m_y;
		Uint32 buttons = g.getMouseState(&m_x, &m_y);
		
		// real x and y, on map
		float r_x = _x + m_x;
		float r_y = _y + m_y;

		if (_edit_mode == EDIT_MODE::TILE) {
			// scroll _edit
			scroll_enum(g, _edit_tile, TILE::TOTAL);
			
			if ((buttons & SDL_BUTTON_LMASK) != 0) {
				g._tile_handler.place_tile(g, _edit_tile, m_x, m_y);
			}

			/*
			case ENEMY_BASIC:
			{
				g._entity_handler._entities.push_back(new EnemyBasic(r_x, r_y));
				break;
			}
			case ENEMY_SHOOTER:
			{
				g._entity_handler._entities.push_back(new EnemyShooter(r_x, r_y));
				break;
			}
			}*/
		}
		else if (_edit_mode == EDIT_MODE::TEX) {
			scroll_enum(g, _edit_tex, TEX::TOTAL);

		}
		else if (_edit_mode == EDIT_MODE::ENTITY) {
			scroll_enum(g, _edit_entity, ENTITIES::TOTAL);

		}

		// place block tile on left click
		if (g._mouse_btn_pressed_this_frame[0])
		{ 
			

		}
		// LEFTclick: delete whatever is in front
		if (g._mouse_btn_pressed_this_frame[2])
		{ 
			if (!g._tile_handler.remove_tile(g, m_x, m_y)) {
				// no tiles removed
				for (int i = ((int)g._entity_handler._entities.size()) - 1; i > -1; --i) {
					auto& e = g._entity_handler._entities[i];
					if (General::general_rect_intersection(r_x, r_y, 0.f, 0.f,
						e->get_x(), e->get_y(), e->get_w(), e->get_h()))
					{
						delete e;
						g._entity_handler._entities.erase(g._entity_handler._entities.begin() + i);
						break;
					}
				}
			}
		}
	}
}



void Camera::draw_edit(Game& g)
{
	draw_grid(g);
	draw_edit_text(g);
}

void Camera::draw_text(Game& g, const char* text, const SDL_Color& color, int x, int y, int scale)
{
	SDL_Surface* surface = TTF_RenderText_Solid(g._font, text, color);

	// now you can convert it into a texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(g._renderer, surface);

	SDL_Rect Message_rect = { x,y, surface->w*scale, surface->h*scale };

	SDL_FreeSurface(surface);

	SDL_RenderCopy(g._renderer, texture, NULL, &Message_rect);

	SDL_DestroyTexture(texture);
}

void Camera::draw_edit_text(Game& g)
{
	
	

	if (_edit_mode == EDIT_MODE::TILE)
	{
		const char* text = "ERROR TILE";

		using namespace TILE;
		switch (_edit_tile) {
		case VOID:
			text = "Void Tile";
			break;
		case BLOCK:
			text = "Block Tile";
			break;
		case DESTRUCTABLE:
			text = "Destructable Tile";
			break;
		case TRI_NE:
			text = "Triangle NE";
			break;
		case TRI_SE:
			text = "Triangle SE";
			break;
		case TRI_NW:
			text = "Triangle NW";
			break;
		case TRI_SW:
			text = "Triangle SW";
			break;
		}
		SDL_Color c = { 0,0,0 };
		draw_text(g, text, c, 0, 0, 3);
		
	}
	else if (_edit_mode == EDIT_MODE::TEX) {
		SDL_Color c = { 0,0,0 };
		draw_text(g, "TEX:", c, 0, 0, 3);
		
		// draw texture of tile
		SDL_Rect rect = { 100, 0, g._cam._grid, g._cam._grid };
		SDL_RenderCopy(g._renderer, g._textures[_edit_tex], NULL, &rect);
	}

	
	return;
	/*
	using namespace EDIT;
	switch (_edit) {
	case BLOCK_TILE:
		text = "Block Tile";
		break;
	case DESTRUCTABLE_TILE:
		text = "Destructible Tile";
		break;
	case TRI_NE:
		text = "Triangle NE";
		break;
	case TRI_SE:
		text = "Triangle SE";
		break;
	case TRI_NW:
		text = "Triangle NW";
		break;
	case TRI_SW:
		text = "Triangle SW";
		break;
	case ENEMY_BASIC:
		text = "Enemy: Basic";
		break;
	case ENEMY_SHOOTER:
		text = "Enemy: Shooter";
		break;
	}
	SDL_Color c = { 0,0,0 };
	draw_text(g, text, c, 0, 0, 3);
	*/
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

void Camera::draw_play(Game& g)
{
	draw_hud(g);
}

void Camera::draw_hud(Game& g)
{
	Player& p = g._entity_handler._p;

	// hp of player
	int hud_x = 25;
	int hud_y = 25;
	int hud_y_increase = 25;
	
	{ // health
		// red health bar
		SDL_SetRenderDrawColor(g._renderer, 255, 0, 0, 127);

		SDL_Rect draw_rect = { hud_x, hud_y, (int)(100.f * ((float)p._hp / p._max_hp)), 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);

		// black boundary
		draw_rect.w = 100;
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 127);
		SDL_RenderDrawRect(g._renderer, &draw_rect);
	}
	hud_y += hud_y_increase;

	{ // coins of player
		SDL_SetRenderDrawColor(g._renderer, 230, 230, 0, 127);

		SDL_Rect draw_rect = { hud_x, hud_y, (int)(10.f * ((float)p._coins / 1)), 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);
	}
	hud_y += hud_y_increase;
	
	{ // left weapon chosen
		int specific_x = hud_x;
		for (int i = 0; i < (int)PLAYER_WEAPON::TOTAL; ++i)
		{	
			SDL_Rect draw_rect = { specific_x, hud_y, 20, 20 };
			
			if ((int)p._left_weapon == i) { // if chosen, mark as green 
				SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, 127);
				SDL_RenderFillRect(g._renderer, &draw_rect);
			}
			SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 127);
			SDL_RenderDrawRect(g._renderer, &draw_rect);
			
			specific_x += 25;

		}
	}
	hud_y += hud_y_increase;
	
	{ // shots player has
		SDL_SetRenderDrawColor(g._renderer, 75, 75, 75, 127);

		SDL_Rect draw_rect = { hud_x, hud_y, 5 * p._shots, 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);
	}
	hud_y += hud_y_increase;
	
	{ // boms player has
		SDL_SetRenderDrawColor(g._renderer, 100, 0, 0, 127);

		SDL_Rect draw_rect = { hud_x, hud_y, 5 * p._bombs, 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);
	}
	hud_y += hud_y_increase;

	{ // bomb throw charge meter
		// green charge (drawn first, BECAUSE: obvious)

		float max_x = 200.f;
		int charge_x = (int)(max_x * (p._bomb_throw_charge / p._bomb_throw_max_charge));
		SDL_Rect draw_rect = { hud_x, hud_y, charge_x, 20  };
		SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, 127);
		SDL_RenderFillRect(g._renderer, &draw_rect);

		// black boundary
		draw_rect.w = (int)max_x;
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 127);
		SDL_RenderDrawRect(g._renderer, &draw_rect);
	}
}