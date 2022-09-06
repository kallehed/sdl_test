#include "Npc.h"
#include "Game.h"
#include "General.h"

MOVING_RECT_TYPES Npc::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::NPC;
}

Npc::Npc(Game& g, NPC_TYPE type, float x, float y)
	: MovingRect(x, y, 64, 74, 0.f), _npc_type(type)
{
	switch (_npc_type) {
	case NPC_TYPE::NPC1:
		
		_text = g._entity_handler._NPC_1_TEXT;
		_total_chars = g._entity_handler._NPC_1_TOTAL_CHARS;
		_tex = TEX::RedBear;

		if (g._save._red_bear_stage > 0) {
			_invisible = true;
		}
		break;
	case NPC_TYPE::NPC2:
		
		_text = g._entity_handler._NPC_2_TEXT;
		_total_chars = g._entity_handler._NPC_2_TOTAL_CHARS;
		_tex = TEX::RedBear;

		if (g._save._red_bear_stage > 1) {
			_invisible = true;
		}
		else {
			g._save._red_bear_stage = 1;
		}
		
		break;
	case NPC_TYPE::NPC3:
		_text = g._entity_handler._NPC_3_TEXT;
		_total_chars = g._entity_handler._NPC_3_TOTAL_CHARS;
		_tex = TEX::RedBear;

		if (g._save._red_bear_stage > 2) {
			_invisible = true;
		}
		else {
			g._save._red_bear_stage = 2;
		}
		break;
	case NPC_TYPE::NPC4:
		_text = g._entity_handler._NPC_4_TEXT;
		_total_chars = g._entity_handler._NPC_4_TOTAL_CHARS;
		_tex = TEX::RedBear;

		if (g._save._red_bear_stage > 3) {
			_invisible = true;
		}
		else {
			g._save._red_bear_stage = 3;
		}
		break;

	case NPC_TYPE::GNOME:
		_text = g._entity_handler._NPC_GNOME_TEXT;
		_total_chars = g._entity_handler._NPC_GNOME_TOTAL_CHARS;
		_tex = TEX::Gnome;
		set_w(32.f * 1.5f);
		set_h(70.f * 1.5f);
		break;
	case NPC_TYPE::GREEN_BOMB_GUY:
		_text = g._entity_handler._NPC_GREEN_BOMB_GUY;
		_total_chars = g._entity_handler._NPC_GREEN_BOMB_GUY_TOTAL_CHARS;
		_tex = TEX::GreenSlime1;
		set_w(32.f * 1.75f);
		set_h(39.f * 1.75f);
		break;
	case NPC_TYPE::SUS_SELLER:
		if (!g._entity_handler._p._ability_to_run) { // Seller text
			_text = g._entity_handler._NPC_SUS_SELLER_TEXT;
			_total_chars = g._entity_handler._NPC_SUS_SELLER_TEXT_TOTAL_CHARS;
		}
		else { // already bought text
			_text = g._entity_handler._NPC_SUS_SELLER_TEXT2;
			_total_chars = g._entity_handler._NPC_SUS_SELLER_TEXT2_TOTAL_CHARS;
		}
		_tex = TEX::SlimeSad;
		set_w(45.f * 2.f);
		set_h(64.f * 2.f);

		if (g._save._sus_seller_stage > 0) {
			_invisible = true;
		}
		break;
	case NPC_TYPE::GUN_GIRL:
		if (g._entity_handler._p._have_l_weapon[L_WEAPON::GUN]) {
			_text = g._entity_handler._NPC_GUN_GIRL_TEXT;
			_total_chars = g._entity_handler._NPC_GUN_GIRL_TEXT_TOTAL_CHARS;
		}
		else {
			_text = g._entity_handler._NPC_GUN_GIRL_TEXT2;
			_total_chars = g._entity_handler._NPC_GUN_GIRL_TEXT2_TOTAL_CHARS;
		}
		_tex = TEX::GirlGreenDress;
		set_w(16.f * 4.f);
		set_h(32.f * 4.f);
		break;
	case NPC_TYPE::TREE_DUDE:
		_tex = TEX::TreeDude;
		_text = g._entity_handler._NPC_TREE_DUDE_TEXT;
		_total_chars = g._entity_handler._NPC_TREE_DUDE_TOTAL_CHARS;
		set_w(44.f * 3.f);
		set_h(46.f * 3.f);
		break;
	case NPC_TYPE::BANANA_MAN:
		_tex = TEX::BananaMan;
		_text = g._entity_handler._NPC_BANANA_MAN_TEXT;
		_total_chars = g._entity_handler._NPC_BANANA_MAN_TOTAL_CHARS;
		set_w(15.f * 3.f);
		set_h(35.f * 3.f);
		break;
	case NPC_TYPE::MOSQUITO:
		_tex = TEX::Mosquito;
		_text = g._entity_handler._NPC_MOSQUITO_TEXT;
		_total_chars = g._entity_handler._NPC_MOSQUITO_TOTAL_CHARS;
		set_w(32.f * 2.f);
		set_h(32.f * 2.f);
		break;
	default:
		std::cout << "ERROR_NPC_TYPE";
		std::cin.get();
		break;
	}
	_temp = _text[0];
	_text[0] = '\0';
}

Npc::~Npc()
{
	_text[_chars_in] = _temp;
	std::cout << "DESTRUCTED!!";
}

bool Npc::logic(Game& g)
{
	move_and_collide<false>(g);

	// if player close, show "Press E"
	Player& p = g._entity_handler._p;

	if (_talking_to) {

		constexpr float threshold = 400.f;
		if (threshold < abs(mid_x() - p.mid_x()) + abs(mid_y() - p.mid_y()))
		{
			_talking_to = false;
		} 
		else if (!_end_of_page)
		{
			_speaking_timer += g._dt;

			int chars_should_be_in = (int)(_speaking_timer) / 45;

			if (chars_should_be_in > _chars_in)
			{
				// go to next character
				_text[_chars_in] = _temp;
				++_chars_in;

				if (_text[_chars_in] == '\0') {
					// at end of page
					_end_of_page = true;
				}

				_temp = _text[_chars_in];
				_text[_chars_in] = '\0';
			}
			else {
				// fast forward
				if (g._keys_frame[SDL_SCANCODE_E]) {
					_speaking_timer += 200.f;
				}
			}
		}
		else {
			// waiting for e press
			_press_e_sign = true;
			if (g._keys_frame[SDL_SCANCODE_E]) {
				if (_chars_in + 1 >= _total_chars) {
					// Done with all pages
					_talking_to = false;

					// SPECIAL FOR SAD_SLIME/SUS_SELLER when bought run
					if (_npc_type == NPC_TYPE::SUS_SELLER && _text == g._entity_handler._NPC_SUS_SELLER_TEXT2) {
						change_y_vel(2.f);
						g._save._sus_seller_stage = 1;
					}
				}
				else {
					// Next page !
					++_chars_in;
					_char_at = _chars_in;
					_end_of_page = false;
					_temp = _text[_char_at];
					_text[_char_at] = '\0';
					_press_e_sign = false;
				}
			}
		}
	}
	else
	{
		constexpr float threshold = 150.f;
		_press_e_sign = threshold > abs(mid_x() - p.mid_x()) + abs(mid_y() - p.mid_y());

		if (_press_e_sign) {
			if (g._keys_frame[SDL_SCANCODE_E]) {
				_talking_to = true;
				_press_e_sign = false;
			}
		}
	}

	return false;
}

void Npc::draw(Game& g)
{
	if (_invisible) return;
	// Sprite and "Press E" box
	{
		bool condition = (!_press_e_sign && _talking_to) && ((((int)_speaking_timer) / 400) % 2 == 0);
		int x_offset = (condition) ?  5 : 0;

		const SDL_Rect rect = { g._cam.convert_x((int)_x) + x_offset, g._cam.convert_y((int)_y),(int)_w,(int)_h };

		draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 25, { 0,0,0,67 });

		SDL_RenderCopy(g._renderer, g._textures[_tex], NULL, &rect);

		// "Press E" Text
		if (_press_e_sign) {
			constexpr int scale = 3;
			const SDL_Rect text_rect = { rect.x - 25 + (int)(10.f * sinf(((float)g._ticks) * 0.1f)), rect.y - 50, g._press_e_w_and_h[0] * scale, g._press_e_w_and_h[1] * scale };
			SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 127);
			SDL_RenderFillRect(g._renderer, &text_rect);
			SDL_RenderCopy(g._renderer, g._press_e_texture, NULL, &text_rect);
		}
	}

	// Talking box
	if (_talking_to) {
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
		int y = (int)((float)g._HEIGHT * 0.575f);
		int x = 267;
		int scale = 20;
		const SDL_Rect box = { x, y, 32*scale, 15*scale };
		SDL_RenderCopy(g._renderer, g._textures[TEX::DialogueBox], NULL, &box);


		constexpr int text_offset = 40;
		g._cam.draw_text(g, _text + _char_at, {0,0,0,255}, x + text_offset, y + text_offset, 3);
	}
}

void Npc::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
