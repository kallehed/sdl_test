#include "Camera.h"
#include "Game.h"
#include "General.h"
#include "EnemyBasic.h"
#include "EnemyShooter.h"
#include "Npc.h"
#include "Portal.h"
#include <fstream>

void Camera::construct(Game& g)
{
	_max_x = TileHandler::_len * _fgrid - g._WIDTH;
	_max_y = TileHandler::_len * _fgrid - g._HEIGHT;

	using namespace CAM_BTN;
	_btns[SAVE].construct(g, 0, 200, "SAVE");
	_btns[LOAD].construct(g, 0, 250, "LOAD");
	_btns[VIEW].construct(g, 0, 300, "VIEW");
	_btns[SET_POS].construct(g, 0, 350, "SET POS");
}

void Camera::shake(Game& g, float time, float intensity)
{
	_shake_timer = time;
	_shake_intensity = intensity;
	
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

	//if (_shake_timer > 0.f) {
		//_shake_timer -= g._dt;

	_x += General::randf01() * _shake_intensity;
	_y += General::randf01() * _shake_intensity;
	_shake_intensity /= 1.5f;
	_shake_intensity = std::max(0.f, _shake_intensity);

	//}

	if (_x < 0) _x = 0;
	if (_y < 0) _y = 0;
	if (_x > _max_x) _x = _max_x;
	if (_y > _max_y) _y = _max_y;
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

	// placing/removing things by clicking + buttons
	{
		// change _edit_mode
		if (g._keys_frame[SDLK_l]) {
			_edit_mode = (EDIT_MODE::EDIT_MODE)(General::mod(_edit_mode + 1, EDIT_MODE::TOTAL ));
		}

		// buttons
		std::array<BTN::BTN, CAM_BTN::TOTAL> btn_states;
		bool any_hovered_on = false;
		for (int i = 0; i < CAM_BTN::TOTAL; ++i) {
			btn_states[i] = _btns[i].logic(g);
			if (btn_states[i] > BTN::NOTHING) {
				any_hovered_on = true;
			}
		}

		if (any_hovered_on) {
			// do btn stuff
			if (btn_states[CAM_BTN::SAVE] == BTN::CLICKED_ON)
			{
				// save button clicked! SAVE
				save_to_file(g);
			}
			else if (btn_states[CAM_BTN::LOAD] == BTN::CLICKED_ON)
			{
				// load button clicked! LOAD
				load_from_file(g, g._level);
			}
			else if (btn_states[CAM_BTN::VIEW] == BTN::CLICKED_ON) {
				_cam_view = (CAM_VIEW::CAM_VIEW)((_cam_view + 1) % CAM_VIEW::TOTAL);
			}
			else if (btn_states[CAM_BTN::SET_POS] == BTN::CLICKED_ON) {
				g._entity_handler._p.set_x(_x + g._WIDTH / 2);
				g._entity_handler._p.set_y(_y + g._HEIGHT / 2);
			}
		}
		else { // no button was hovered over
			// place/delete things stuff

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
					g._tile_handler.place_tex(g, _edit_tex, m_x, m_y);
				}
			}
			else if (_edit_mode == EDIT_MODE::TEX) {
				scroll_enum(g, _edit_tex, TEX::TOTAL);
				if ((buttons & SDL_BUTTON_LMASK) != 0) {
					g._tile_handler.place_tex(g, _edit_tex, m_x, m_y);
				}
			}
			else if (_edit_mode == EDIT_MODE::ENTITY) {
				scroll_enum(g, _edit_entity, ENTITIES::TOTAL);

				// left click to place entity
				if (g._mouse_btn_pressed_this_frame[0])
				{
					using namespace ENTITIES;
					switch (_edit_entity) {
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
					case NPC:
					{
						g._entity_handler._draw_entities.push_back(new Npc(r_x, r_y));
						break;
					}
					case PORTAL:
					{
						g._entity_handler._draw_entities.push_back(new Portal(r_x, r_y, g._level+1, "Error_Portal_Name", "Error_Destination_Name"));
						break;
					}
					}
				}

			}

			// LEFTclick: delete whatever is in front
			if (g._mouse_btn_pressed_this_frame[2])
			{
				bool entity_removed = false;
				for (int i = ((int)g._entity_handler._entities.size()) - 1; i > -1; --i) {
					auto& e = g._entity_handler._entities[i];
					if (General::general_rect_intersection(r_x, r_y, 0.f, 0.f,
						e->get_x(), e->get_y(), e->get_w(), e->get_h()))
					{
						delete e;
						g._entity_handler._entities.erase(g._entity_handler._entities.begin() + i);
						entity_removed = true;
						break;
					}
				}

				// no entities removed
				if (!entity_removed) {
					bool entity_removed2 = false;
					for (int i = ((int)g._entity_handler._draw_entities.size()) - 1; i > -1; --i) {
						auto& e = g._entity_handler._draw_entities[i];
						if (General::general_rect_intersection(r_x, r_y, 0.f, 0.f,
							e->get_x(), e->get_y(), e->get_w(), e->get_h()))
						{
							delete e;
							g._entity_handler._draw_entities.erase(g._entity_handler._draw_entities.begin() + i);
							entity_removed2 = true;
							break;
						}
					}
					if (!entity_removed2) {
						g._tile_handler.remove_tile(g, m_x, m_y);
					}
				}
			}
		}
	}
}

void Camera::save_to_file(Game& g)
{
	/* FILE FORMAT:
	* 
	* each statement should end with a \n
	* 
	* TILE
	* attributes: i, j, tile, tex
	* END
	* 
	* ENTITY
	* attributes: i, j, type, portal_destination, portal_name, portal_destination_name
	* type : EnemyBasic, EnemyShooter, Npc, Player, Portal
	* 
	* 
	*/

	std::cout << "SAVE\n";

	// Create and open a text file
	std::string text_file = "levels/level_" + std::to_string((int)g._level) + ".txt";
	std::ofstream f(text_file);

	// (TILE)s
	for (int i = 0; i < g._tile_handler._len; ++i) {
		for (int j = 0; j < g._tile_handler._len; ++j) {
			f << "TILE\n";
			f << "i\n" << i << "\n";
			f << "j\n" << j << "\n";
			f << "tile\n" << std::to_string(g._tile_handler.get_tile_type(i, j)) << "\n";
			f << "tex\n" << std::to_string(g._tile_handler.get_tile_tex(i, j)) << "\n";
			f << "END\n";
		}
	}

	// (ENTITY)s
	for (auto e : g._entity_handler._entities) {
		if (dynamic_cast<Enemy*>(e)) {
			f << "ENTITY\n";
			f << "i\n" << std::to_string(g._cam.convert_y_to_i(e->_y)) << "\n";
			f << "j\n" << std::to_string(g._cam.convert_x_to_j(e->_x)) << "\n";
			if (dynamic_cast<EnemyBasic*>(e)) {
				f << "type\n" << "EnemyBasic\n";
			}
			else if (dynamic_cast<EnemyShooter*>(e)) {
				f << "type\n" << "EnemyShooter\n";
			}
			f << "END\n";
		}
	}
	// Draw Entities
	for (auto e : g._entity_handler._draw_entities) {
		if (dynamic_cast<Npc*>(e) || dynamic_cast<Portal*>(e)) {
			f << "ENTITY\n";
			f << "i\n" << std::to_string(g._cam.convert_y_to_i(e->_y)) << "\n";
			f << "j\n" << std::to_string(g._cam.convert_x_to_j(e->_x)) << "\n";

			if (dynamic_cast<Npc*>(e)) {
				f << "type\n" << "Npc\n";
			}
			else if (dynamic_cast<Portal*>(e)) {
				f << "type\n" << "Portal\n";
				Portal* portal = dynamic_cast<Portal*>(e);
				f << "portal_destination\n" << std::to_string(portal->_destination_level) << "\n";
				f << "portal_name\n" << portal->_name << "\n";
				f << "portal_destination_name\n" << portal->_destination_name << "\n";
			}
			f << "END\n";
		}
	}

	// Player position
	{
		f << "ENTITY\n";
		f << "i\n" << std::to_string(g._cam.convert_y_to_i(g._entity_handler._p._y)) << "\n";
		f << "j\n" << std::to_string(g._cam.convert_x_to_j(g._entity_handler._p._x)) << "\n";
		f << "type\n" << "Player\n";
		f << "END\n";
	}

	// Close the file
	f.close();
}

void Camera::load_from_file(Game& g, int level)
{
	std::cout << "LOAD\n";

	// reset all tiles
	g._tile_handler.reset_all(g);

	// delete all entities.
	{
		for (const auto& e : g._entity_handler._entities) {
			delete e;
		}
		g._entity_handler._entities.clear();

		for (const auto& e : g._entity_handler._draw_entities) {
			delete e;
		}
		g._entity_handler._draw_entities.clear();
	}

	std::string t;

	// Read from the text file
	std::string text_file = "levels/level_" + std::to_string(level) + ".txt";

	std::ifstream f(text_file);

	bool player_has_been_placed_by_portal = false;

	int highest_tile_i = 20;
	int highest_tile_j = 20;

	// Use a while loop together with the getline() function to read the file line by line
	while (std::getline(f, t)) {
		// Output the text from the file
		if (t == "TILE") {
			int i = 0, j = 0;
			TILE::TILE tile = TILE::BLOCK;
			TEX::TEX tex = TEX::FireMagic;

			while (t != "END") {
				std::getline(f, t);
				if (t == "i") {
					std::getline(f, t);
					i = std::stoi(t);
					
				}
				else if (t == "j") {
					std::getline(f, t);
					j = std::stoi(t);
					
				}
				else if (t == "tile") {
					std::getline(f, t);
					tile = (TILE::TILE)std::stoi(t);
				}
				else if (t == "tex") {
					std::getline(f, t);
					tex = (TEX::TEX)std::stoi(t);
				}
			}
			_ASSERT(i >= 0 && j >= 0 && i < g._tile_handler._len && j < g._tile_handler._len);
			g._tile_handler._tiles[i][j] = tile;
			g._tile_handler._texs[i][j] = tex;

			if (tile != TILE::VOID) {
				if (i > highest_tile_i) {
					highest_tile_i = i;
				}
				if (j > highest_tile_j) {
					highest_tile_j = j;
				}
			}
		}
		else if (t == "ENTITY") {
			int i = 0, j = 0;
			std::string type = "EnemyBasic";

			// PORTAL STUFF
			int portal_destination = 0;
			std::string portal_name = "Error_Name";
			std::string portal_destination_name = "Error_Destination_name";

			while (t != "END") {
				std::getline(f, t);
				if (t == "i") {
					std::getline(f, t);
					i = std::stoi(t);
				}
				else if (t == "j") {
					std::getline(f, t);
					j = std::stoi(t);
				}
				else if (t == "type") {
					std::getline(f, t);
					type = t;
				}
				else if (t == "portal_destination") {
					std::getline(f, t);
					portal_destination = std::stoi(t);
				}
				else if (t == "portal_name") {
					std::getline(f, t);
					portal_name = t;
				}
				else if (t == "portal_destination_name") {
					std::getline(f, t);
					portal_destination_name = t;
				}
			}
			if (type == "EnemyBasic") {
				EnemyBasic* e = new EnemyBasic(j * _fgrid, i * _fgrid);
				g._entity_handler._entities.emplace_back(e);
			}
			else if (type == "EnemyShooter") {
				EnemyShooter* e = new EnemyShooter(j * _fgrid, i * _fgrid);
				g._entity_handler._entities.emplace_back(e);
			}
			else if (type == "Npc") {
				Npc* e = new Npc(j * _fgrid, i * _fgrid);
				g._entity_handler._draw_entities.emplace_back(e);
			}
			else if (type == "Portal") {
				Portal* e = new Portal(j * _fgrid, i * _fgrid, portal_destination, portal_name, portal_destination_name);
				g._entity_handler._draw_entities.emplace_back(e);

				if (portal_name == g._destination_portal) {
					g._entity_handler._p.set_x(j* _fgrid);
					g._entity_handler._p.set_y(i* _fgrid);
					player_has_been_placed_by_portal = true;
				}
			}
			else if (type == "Player") {
				if (player_has_been_placed_by_portal == false) {
					g._entity_handler._p.set_x(j * _fgrid);
					g._entity_handler._p.set_y(i * _fgrid);
				}
			}
		}
	}
	 // TODO:: FIX SO ONLY TILES THAT ARE NOT VOID SHOULD AFFECT MAX X OR Y.
	g._cam._max_x = (highest_tile_j + 1) * g._cam._fgrid - g._WIDTH;
	g._cam._max_y = (highest_tile_i + 1) * g._cam._fgrid - g._HEIGHT;

	// Close the file
	f.close();
}

void Camera::draw_edit(Game& g)
{
	draw_grid(g);
	draw_edit_text(g);
}

void Camera::draw_text(Game& g, const char* text, const SDL_Color& color, int x, int y, int scale)
{
	if (text[0] == '\0') {
		return; // don't do anything if it's empty
	}
	SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(g._font, text, color, 0);

	// now you can convert it into a texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(g._renderer, surface);

	SDL_Rect Message_rect = { x,y, surface->w*scale, surface->h*scale };

	SDL_FreeSurface(surface);
	SDL_RenderCopy(g._renderer, texture, NULL, &Message_rect);

	SDL_DestroyTexture(texture);
}
void Camera::draw_text_background(Game& g, const char* text, const SDL_Color& color, const SDL_Color& color2, int x, int y, int scale)
{
	SDL_Surface* surface = TTF_RenderText_Solid(g._font, text, color);

	// now you can convert it into a texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(g._renderer, surface);

	SDL_Rect Message_rect = { x,y, surface->w * scale, surface->h * scale };

	SDL_FreeSurface(surface);
	SDL_SetRenderDrawColor(g._renderer, color2.r, color2.b, color2.g, color2.a);
	SDL_RenderFillRect(g._renderer, &Message_rect);
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
		draw_text_background(g, text, c, {255,255,255,255}, 0, 0, 3);
		
	}
	else if (_edit_mode == EDIT_MODE::TEX)
	{
		SDL_Color c = { 0,0,0 };
		draw_text_background(g, "TEX:", c, {255,255,255,255}, 0, 0, 3);
		
		// draw texture of tile
		SDL_Rect rect = { 100, 0, g._cam._grid, g._cam._grid };
		SDL_RenderCopy(g._renderer, g._textures[_edit_tex], NULL, &rect);
	}
	else if (_edit_mode == EDIT_MODE::ENTITY)
	{
		const char* text = "ERROR ENTITY";

		using namespace ENTITIES;
		switch (_edit_entity) {
		case ENEMY_BASIC:
			text = "Enemy: Basic";
			break;
		case ENEMY_SHOOTER:
			text = "Enemy: Shooter";
			break;
		case NPC:
			text = "Npc";
			break;
		case PORTAL:
			text = "Portal";
			break;
		}
		SDL_Color c = { 0,0,0 };
		draw_text_background(g, text, c, {255,255,255,255}, 0, 0, 3);
	}

	// other edit draw stuff
	for (int i = 0; i < CAM_BTN::TOTAL; ++i) {
		_btns[i].draw(g);
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

void Camera::draw_play(Game& g)
{
	draw_hud(g);
}

void Camera::draw_hud(Game& g)
{
	Player& p = g._entity_handler._p;
	int a = 180; // alpha

	// hp of player
	int hud_x = 25;
	int hud_y = 25;
	int hud_y_increase = 25;
	
	{ // health
		// red health bar
		SDL_SetRenderDrawColor(g._renderer, 255, 0, 0, a);

		SDL_Rect draw_rect = { hud_x, hud_y, (int)(100.f * ((float)p._hp / p._max_hp)), 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);

		// black boundary
		draw_rect.w = 100;
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
		SDL_RenderDrawRect(g._renderer, &draw_rect);
	}
	hud_y += hud_y_increase;

	{ // coins of player
		SDL_SetRenderDrawColor(g._renderer, 230, 230, 0, a);

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
				SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, a);
				SDL_RenderFillRect(g._renderer, &draw_rect);
			}
			SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
			SDL_RenderDrawRect(g._renderer, &draw_rect);
			
			specific_x += 25;

		}
	}
	hud_y += hud_y_increase;
	
	{ // left weapon "ammo" left
		if (p._left_weapon == PLAYER_WEAPON::FIRE_MAGIC)
		{
			float dim = 10.f; // divide width by this

			int charge_x = (int)(p._fire_magic_current/dim);
			SDL_Rect draw_rect = { hud_x, hud_y, charge_x, 20 };

			/*SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, a);
			SDL_RenderFillRect(g._renderer, &draw_rect);*/

			SDL_SetTextureAlphaMod(g._textures[TEX::FireMagic], 255);
			SDL_RenderCopy(g._renderer, g._textures[TEX::FireMagic], NULL, &draw_rect);

			// black boundary
			draw_rect.w = (int)(p._fire_magic_max/dim);
			SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
			SDL_RenderDrawRect(g._renderer, &draw_rect);
		}
		else if (p._left_weapon == PLAYER_WEAPON::GUN)
		{ // shots player has
			SDL_SetRenderDrawColor(g._renderer, 75, 75, 75, a);

			SDL_Rect draw_rect = { hud_x, hud_y, 5 * p._shots, 20 };
			SDL_RenderFillRect(g._renderer, &draw_rect);
		}
	}
	hud_y += hud_y_increase;
	
	{ // boms player has
		SDL_SetRenderDrawColor(g._renderer, 100, 0, 0, a);

		SDL_Rect draw_rect = { hud_x, hud_y, 5 * p._bombs, 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);
	}
	hud_y += hud_y_increase;

	{ // bomb throw charge meter
		// green charge (drawn first, BECAUSE: obvious)

		float max_x = 200.f;
		int charge_x = (int)(max_x * (p._bomb_throw_charge / p._bomb_throw_max_charge));
		SDL_Rect draw_rect = { hud_x, hud_y, charge_x, 20  };
		SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, a);
		SDL_RenderFillRect(g._renderer, &draw_rect);

		// black boundary
		draw_rect.w = (int)max_x;
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
		SDL_RenderDrawRect(g._renderer, &draw_rect);
	}
}