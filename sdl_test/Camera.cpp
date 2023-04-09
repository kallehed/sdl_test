#include "Camera.h"
#include "Game.h"
#include "General.h"
#include "EnemyBasic.h"
#include "EnemyShooter.h"
#include "EnemyDash.h"
#include "EnemyJump.h"
#include "Npc.h"
#include "Portal.h"
#include "Bonfire.h"
#include "Chest.h"
#include "Buyable.h"
#include "Door.h"
#include "BossBody.h"
#include "Boss2.h"
#include <fstream>
#include <sstream>

void Camera::construct(Game& g)
{
	_max_x = TileHandler::_len * _fgrid - g._WIDTH;
	_max_y = TileHandler::_len * _fgrid - g._HEIGHT;

	using namespace CAM_BTN;
	_btns[NEXT].construct(g, 0, 200, "NEXT");
	_btns[PREV].construct(g, 0, 250, "PREV");
	_btns[VIEW].construct(g, 0, 300, "VIEW");
	_btns[SET_POS].construct(g, 0, 350, "SET POS");
}

void Camera::shake(Game& g, float divider, float intensity)
{
	_shake_divider = divider; // completly replace possible previous shaking
	_shake_intensity += intensity;

	vibrate_phone((int)((intensity > 20.f) ? (intensity / 2.f) : (intensity)));
}

int Camera::convert_x(int x) const {
	return int(std::ceil(((double)x - _x)));
}

int Camera::convert_y(int y) const {
	return (double)y - _y;
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
	double tarx = p.mid_x() - g._WIDTH / 2.f;
	double tary = p.mid_y() - g._HEIGHT / 2.f;

	double speed = 0.2f;
	double x_change = (tarx - _x) * speed;
	double y_change = (tary - _y) * speed;

	// limit too small camera movements
	static const double mov_boundary = 1.f;
	//if (std::abs(x_change) > mov_boundary)
		_x += x_change;
	//if (std::abs(y_change) > mov_boundary)
		_y += y_change;
		
	//if (_shake_timer > 0.f) {
		//_shake_timer -= g._dt;

	_x += (General::randf01()*2.0 - 1.0) * _shake_intensity;
	_y += (General::randf01()*2.0 - 1.0) * _shake_intensity;
	_shake_intensity /= _shake_divider;
	_shake_intensity = std::max(0.0, _shake_intensity);

	//}

	if (_x < 0) _x = 0;
	if (_y < 0) _y = 0;

	// player should ALWAYS be on screen, allows pushing of screen if possible.
	double max_x = std::max(_max_x, (double)p._x + p._w - g._WIDTH);
	double max_y = std::max(_max_y, (double)p._y + p._h - g._HEIGHT);
	if (_x > max_x) _x = max_x;
	if (_y > max_y) _y = max_y;
	
	// make coordinates NOT too floaty
	/*const float dec = 1.f;
	_x *= dec; _y *= dec;
	_x = std::round(_x);
	_y = std::round(_y);

	_x /= dec; _y /= dec;*/
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
	auto& keys = g._keys_down;
	double speed = (!keys[SDL_SCANCODE_LSHIFT]) ? 0.65 : 2.3;
	
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
		if (g._keys_frame[SDL_SCANCODE_L]) {
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
			if (btn_states[CAM_BTN::NEXT] == BTN::CLICKED_ON)
			{
				// save button clicked! SAVE
				save_to_file(g);
				load_from_file(g, g._level + 1);
				
			}
			else if (btn_states[CAM_BTN::PREV] == BTN::CLICKED_ON)
			{
				// load button clicked! LOAD
				save_to_file(g);
				load_from_file(g, g._level - 1);
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
			int m_x = g._mouse_pos[0];
			int m_y = g._mouse_pos[1];

			// real x and y, on map
			float r_x = _x + m_x;
			float r_y = _y + m_y;

			if (_edit_mode == EDIT_MODE::TILE) {
				// scroll _edit
				scroll_enum(g, _edit_tile, TILE::TOTAL);

				if (g._mouse_btn_down[0]) { // left mouse down
					g._tile_handler.place_tile(g, _edit_tile, m_x, m_y);
					g._tile_handler.place_tex(g, _edit_tex, m_x, m_y);
				}
			}
			else if (_edit_mode == EDIT_MODE::TEX) {
				scroll_enum(g, _edit_tex, TEX::TOTAL);
				if (g._mouse_btn_down[0]) {
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
					case ENEMY_DASH:
					{
						g._entity_handler._entities.push_back(new EnemyDash(r_x, r_y));
						break;
					}
					case ENEMY_JUMP:
					{
						g._entity_handler._entities.push_back(new EnemyJump(r_x, r_y));
						break;
					}
					case NPC:
					{
						g._entity_handler._draw_entities.push_back(new Npc(g, NPC_TYPE::NPC1, r_x, r_y));
						break;
					}
					case PORTAL:
					{
						g._entity_handler._draw_entities.push_back(new Portal(r_x, r_y, g._level+1, "Error_Portal_Name", "Error_Destination_Name"));
						break;
					}
					case BONFIRE:
					{
						g._entity_handler._draw_entities.push_back(new Bonfire(r_x, r_y));
						break;
					}
					case CHEST:
					{
						g._entity_handler._draw_entities.push_back(new Chest(-1,r_x, r_y, 10, CHEST_TYPE::COIN_CHEST));
						break;
					}
					case BUYABLE:
					{
						g._entity_handler._draw_entities.push_back(new Buyable(-1,-420,BUYABLE_TYPE::FASTER_FIRE_RECHARGE, r_x, r_y));
						break;
					}
					case BOSS_HEAD:
					{
						g._entity_handler._entities.push_back(new BossBody(-1,r_x, r_y));
						break;
					}
					case HP_UPGRADE:
					{
						g._entity_handler._draw_entities.push_back(new Chest(-1, r_x, r_y, 5, CHEST_TYPE::HP_UPGRADE));
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
						e->x(), e->y(), e->w(), e->h()))
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
							e->x(), e->y(), e->w(), e->h()))
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
	* BACKGROUND_TILE
	* followed by TEX::TEX
	* 
	* MUSIC
	* followed by MUS::_
	* 
	*/

	std::cout << "SAVE\n";

	// Create and open a text file
	std::string text_file = "data/levels/level_" + std::to_string((int)g._level) + ".txt";
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
		if (dynamic_cast<Enemy*>(e) || dynamic_cast<BossBody*>(e) || dynamic_cast<Boss2*>(e)) {
			f << "ENTITY\n";
			f << "i\n" << std::to_string(g._cam.convert_y_to_i(e->_y)) << "\n";
			f << "j\n" << std::to_string(g._cam.convert_x_to_j(e->_x)) << "\n";
			if (dynamic_cast<EnemyBasic*>(e)) {
				f << "type\n" << "EnemyBasic\n";
			}
			else if (dynamic_cast<EnemyShooter*>(e)) {
				f << "type\n" << "EnemyShooter\n";
			}
			else if (dynamic_cast<EnemyDash*>(e)) {
				f << "type\n" << "EnemyDash\n";
			}
			else if (dynamic_cast<EnemyJump*>(e)) {
				f << "type\n" << "EnemyJump\n";
			}
			else if (dynamic_cast<BossBody*>(e)) {
				f << "type\n" << "BossBody\n";
			}
			else if (dynamic_cast<Boss2*>(e)) {
				f << "type\n" << "Boss2\n";
			}
			f << "END\n";
		}
	}
	// Draw Entities
	for (auto e : g._entity_handler._draw_entities) {
		if (dynamic_cast<Npc*>(e) || dynamic_cast<Portal*>(e) || dynamic_cast<Bonfire*>(e) || dynamic_cast<Chest*>(e) || dynamic_cast<Buyable*>(e) || dynamic_cast<Door*>(e)) {
			f << "ENTITY\n";
			f << "i\n" << std::to_string(g._cam.convert_y_to_i(e->_y)) << "\n";
			f << "j\n" << std::to_string(g._cam.convert_x_to_j(e->_x)) << "\n";

			if (dynamic_cast<Npc*>(e)) {
				f << "type\n" << "Npc\n";
				Npc* npc = static_cast<Npc*>(e);
				f << "npc_type\n" << std::to_string((int)npc->_npc_type) << "\n";
			}
			else if (dynamic_cast<Portal*>(e)) {
				f << "type\n" << "Portal\n";
				Portal* portal = static_cast<Portal*>(e);
				f << "portal_destination\n" << std::to_string(portal->_destination_level) << "\n";
				f << "portal_name\n" << portal->_name << "\n";
				f << "portal_destination_name\n" << portal->_destination_name << "\n";
			}
			else if (dynamic_cast<Bonfire*>(e)) {
				f << "type\n" << "Bonfire\n";
			}
			else if (dynamic_cast<Chest*>(e)) {
				Chest* chest = static_cast<Chest*>(e);
				f << "type\n" << "Chest\n";
				f << "chest_amount\n" << std::to_string(chest->_chest_amount) << "\n";
				f << "chest_type\n" << std::to_string((int)chest->_type) << "\n";
			}
			else if (dynamic_cast<Buyable*>(e)) {
				Buyable* buyable = (Buyable*)e;
				f << "type\n" << "Buyable\n";
				f << "buyable_cost\n" << std::to_string(buyable->_cost) << "\n";
				f << "buyable_type\n" << std::to_string((int)buyable->_type) << "\n";
			}
			else if (dynamic_cast<Door*>(e)) {
				Door* door = (Door*)e;
				f << "type\n" << "Door\n";
				f << "door_type\n" << std::to_string((int)door->_type) << "\n";
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

	// background tile
	{
		f << "BACKGROUND_TILE\n";
		f << std::to_string(g._tile_handler._background_tile) << "\n";
		f << "END\n";
	}

	// background music
	{
		f << "MUSIC\n";
		f << std::to_string(g._current_music) << "\n";
		f << "END\n";
	}

	// Close the file
	f.close();
}

void Camera::load_from_file(Game& g, int level)
{
	std::cout << "LOAD\n";

	g._level = level;

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

		for (const auto& e : g._entity_handler._particles) {
			delete e;
		}
		g._entity_handler._particles.clear();
	}

	std::string t;

	// Read from the text file
	std::string text_file = "data/levels/level_" + std::to_string(level) + ".txt";

#ifdef __ANDROID__
	SDL_RWops* io = SDL_RWFromFile(text_file.c_str(), "r");
	if (io == NULL) {
		SDL_Log("testing KALLE FAILED TO LOAD RWOPS FILE");
		return;
	}
	const int len = 1000000;
	char* name = new char[len]();
	io->read(io, name, len, 1);
	std::stringstream f;
	f << name;
#else
	std::ifstream f(text_file);
#endif

	bool player_has_been_placed_by_portal = false;

	int highest_tile_i = 20;
	int highest_tile_j = 20;


	// General variables for all categories
	int i = 0, j = 0;
	TILE::TILE tile = TILE::BLOCK;
	TEX::TEX tex = TEX::FireMagic;

	std::string type = "EnemyBasic";

	// PORTAL STUFF
	int portal_destination = 0;
	std::string portal_name = "Error_Name";
	std::string portal_destination_name = "Error_Destination_name";

	// NPC STUFF
	NPC_TYPE::_ npc_type = NPC_TYPE::NPC1;

	// ONETIME INDEX, used by Chest + Buyable
	int onetime_index = 0;
	auto& onetimes = g._save._onetimes;

	// CHEST STUFF
	int chest_amount = 0;
	CHEST_TYPE::_ chest_type = CHEST_TYPE::COIN_CHEST;

	// BUYABLE STUFF
	int buyable_cost = -420;
	BUYABLE_TYPE::_ buyable_type = BUYABLE_TYPE::FASTER_FIRE_RECHARGE;

	// DOOR STUFF
	DOOR_TYPE::_ door_type = DOOR_TYPE::OH_HEY;

	// Use a while loop together with the getline() function to read the file line by line
	while (std::getline(f, t)) {
		//SDL_Log("KALLE log: %s", t.c_str());
		// Output the text from the file
		if (t == "TILE") {
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
			//_ASSERT(i >= 0 && j >= 0 && i < g._tile_handler._len && j < g._tile_handler._len);
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
		else if (t == "BACKGROUND_TILE") {
			std::getline(f, t);
			g._tile_handler._background_tile = (TEX::TEX)std::stoi(t);
			std::getline(f, t); // move past end
		}
		else if (t == "MUSIC") {
			std::getline(f, t);
			g.play_music((MUS::_)std::stoi(t));
			std::getline(f, t);
		}
		else if (t == "ENTITY") {
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
				else if (t == "npc_type") {
					std::getline(f, t);
					npc_type = (NPC_TYPE::_)std::stoi(t);
				}
				else if (t == "chest_amount") {
					std::getline(f, t);
					chest_amount = std::stoi(t);
				}
				else if (t == "buyable_cost") {
					std::getline(f, t);
					buyable_cost = std::stoi(t);
				}
				else if (t == "buyable_type") {
					std::getline(f, t);
					buyable_type = (BUYABLE_TYPE::_)std::stoi(t);
				}
				else if (t == "door_type") {
					std::getline(f, t);
					door_type = (DOOR_TYPE::_)std::stoi(t);
				}
				else if (t == "chest_type") {
					std::getline(f, t);
					chest_type = (CHEST_TYPE::_)std::stoi(t);
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
			else if (type == "EnemyDash") {
				EnemyDash* e = new EnemyDash(j * _fgrid, i * _fgrid);
				g._entity_handler._entities.emplace_back(e);
			}
			else if (type == "EnemyJump") {
				EnemyJump* e = new EnemyJump(j * _fgrid, i * _fgrid);
				g._entity_handler._entities.emplace_back(e);
			}
			else if (type == "Npc") {
				Npc* e = new Npc(g, npc_type, j * _fgrid, i * _fgrid);
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
			else if (type == "Bonfire") {
				Bonfire* e = new Bonfire(j * _fgrid, i * _fgrid);
				g._entity_handler._draw_entities.emplace_back(e);
			}
			else if (type == "Chest") {
				if (onetimes.find({level, onetime_index}) == onetimes.end()) {
					Chest* e = new Chest(onetime_index, j * _fgrid, i * _fgrid, chest_amount, chest_type);
					g._entity_handler._draw_entities.emplace_back(e);
				}
				++onetime_index;
			}
			else if (type == "Buyable") {
				if (onetimes.find({ level, onetime_index }) == onetimes.end()) {
					Buyable* e = new Buyable(onetime_index, buyable_cost, buyable_type, j * _fgrid, i * _fgrid);
					g._entity_handler._draw_entities.emplace_back(e);
				}
				++onetime_index;
			}
			else if (type == "Door") {
				if (onetimes.find({ level, onetime_index }) == onetimes.end()) {
					Door* e = new Door(j * _fgrid, i * _fgrid, onetime_index, door_type);
					g._entity_handler._draw_entities.emplace_back(e);
				}
				++onetime_index;
			}
			else if (type == "BossBody") {
				if (onetimes.find({ level, onetime_index }) == onetimes.end()) {
					BossBody* e = new BossBody(onetime_index,j * _fgrid, i * _fgrid); // Head constructor
					g._entity_handler._entities.emplace_back(e);
				}
				++onetime_index;
			}
			else if (type == "Boss2") {
				if (onetimes.find({ level, onetime_index }) == onetimes.end()) {
					Boss2* e = new Boss2(onetime_index, j * _fgrid, i * _fgrid);
					g._entity_handler._entities.emplace_back(e);
				}
				++onetime_index;
			}
			else if (type == "Player") {
				if (player_has_been_placed_by_portal == false) {
					g._entity_handler._p.set_x(j * _fgrid);
					g._entity_handler._p.set_y(i * _fgrid);
				}
			}
		}
	}

	g._cam._max_x = (highest_tile_j + 1) * g._cam._fgrid - g._WIDTH;
	g._cam._max_y = (highest_tile_i + 1) * g._cam._fgrid - g._HEIGHT;

#ifndef __ANDROID__
	// Close the file
	f.close();
#else
	delete [] name;
#endif
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
		case ENEMY_DASH:
			text = "Enemy: Dash";
			break;
		case ENEMY_JUMP:
			text = "Enemy: Jump";
			break;
		case NPC:
			text = "Npc";
			break;
		case PORTAL:
			text = "Portal";
			break;
		case BONFIRE:
			text = "Bonfire";
			break;
		case CHEST:
			text = "Chest";
			break;
		case BUYABLE:
			text = "Buyable";
			break;
		case BOSS_HEAD:
			text = "BossHead";
			break;
		case HP_UPGRADE:
			text = "Hp Upgrade";
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

		// black area
		int max_width = 2 * p._max_hp;
		SDL_Rect black_rect = { hud_x, hud_y, max_width, 20 };
		
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
		SDL_RenderFillRect(g._renderer, &black_rect);

		// red health bar
		SDL_SetRenderDrawColor(g._renderer, 255, 0, 0, a);
		black_rect.w = (int)(max_width * ((float)p._hp / p._max_hp));
		SDL_RenderFillRect(g._renderer, &black_rect);

		// black borders
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
		black_rect.w = max_width;
		SDL_RenderDrawRect(g._renderer, &black_rect);
	}
	hud_y += hud_y_increase;


	if (p._ability_to_run)
	{
		// red health bar
		SDL_SetRenderDrawColor(g._renderer, 0,255, 0, a);

		SDL_Rect draw_rect = { hud_x, hud_y, (int)(100.f * ((float)p._run_current / p._run_max)), 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);

		// black boundary
		draw_rect.w = 100;
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
		SDL_RenderDrawRect(g._renderer, &draw_rect);


		hud_y += hud_y_increase;
	}

	{ // coins of player
		SDL_SetRenderDrawColor(g._renderer, 230, 230, 0, a);
		SDL_Rect draw_rect = { hud_x,hud_y,25,25 };
		SDL_RenderCopy(g._renderer, g._textures[TEX::Coin], NULL,  &draw_rect);
		draw_text(g, std::to_string(p._coins).c_str(), { 0,0,0,(Uint8)a }, hud_x + draw_rect.w, hud_y - 7, 2);
		//SDL_Rect draw_rect = { hud_x, hud_y, (int)(10.f * ((float)p._coins / 1)), 20 };
		//SDL_RenderFillRect(g._renderer, &draw_rect);
	}
	hud_y += hud_y_increase;
	
	{ // left weapon chosen
		int specific_x = hud_x;
		for (int i = 0; i < L_WEAPON::TOTAL; ++i)
		{	
			if (p._have_l_weapon[i])
			{
				SDL_Rect draw_rect = { specific_x, hud_y, 20, 20 };

				// if chosen, mark as green
				if (p._left_weapon == i)
				{ 
					SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, a);
					SDL_RenderFillRect(g._renderer, &draw_rect);
				}
				SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
				SDL_RenderDrawRect(g._renderer, &draw_rect);

				specific_x += 25;
			}
		}
	}
	hud_y += hud_y_increase;
	
	{ // left weapon "ammo" left
		if (p._left_weapon == L_WEAPON::FIRE_MAGIC)
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
		else if (p._left_weapon == L_WEAPON::GUN)
		{ // shots player has
			SDL_SetRenderDrawColor(g._renderer, 75, 75, 75, a);

			SDL_Rect draw_rect = { hud_x, hud_y, 5 * p._shots, 20 };
			SDL_RenderFillRect(g._renderer, &draw_rect);
		}
	}
	hud_y += hud_y_increase;
	
	{ // bombs player has
		SDL_SetRenderDrawColor(g._renderer, 100, 0, 0, a);

		SDL_Rect draw_rect = { hud_x, hud_y, 5 * p._bombs, 20 };
		SDL_RenderFillRect(g._renderer, &draw_rect);
	}
	hud_y += hud_y_increase;

	{ // bomb throw charge meter
		// green charge (drawn first, BECAUSE: obvious)

		float max_x = 200.f;
		int charge_x = 0;
		SDL_Color color = { 0,255,0,(Uint8)a };
		if (p._bomb_recharge_timer > 0.f) {
			charge_x = (int)(max_x * (p._bomb_recharge_timer / p._bomb_recharge_time));
			color.r = 255;
		}
		else {
			charge_x = (int)(max_x * (p._bomb_throw_charge / p._bomb_throw_max_charge));
		}
		
		SDL_Rect draw_rect = { hud_x, hud_y, charge_x, 20  };
		SDL_SetRenderDrawColor(g._renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(g._renderer, &draw_rect);

		// black boundary
		draw_rect.w = (int)max_x;
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, a);
		SDL_RenderDrawRect(g._renderer, &draw_rect);
	}

	// Teleport to other level through portal, black screen
	if (g._change_level) {
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 100);
		constexpr const SDL_Rect draw_rect = { 0,0, 10000, 10000 };
		SDL_RenderFillRect(g._renderer, &draw_rect);
	}
}
