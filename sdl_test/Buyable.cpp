#include "Buyable.h"
#include "Game.h"

MOVING_RECT_TYPES Buyable::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::BUYABLE;
}

Buyable::Buyable(int onetime_index, int cost, BUYABLE_TYPE type, float x, float y)
	: MovingRect(x, y, 60, 60, 1.f),
	_onetime_index(onetime_index),
	_cost(cost),
	_type(type)
{
	using enum BUYABLE_TYPE;
	switch (_type) {
	case FASTER_FIRE_RECHARGE:
		_descriptive_text = "Faster Fire\nRecharge";
		break;
	case LARGER_FIRE_LIMIT:
		_descriptive_text = "Maximum Total\nFire Increase";
		break;
	case INCREASED_FIRE_DAMAGE:
		_descriptive_text = "Increased Fire\nDamage";
		break;
	case LARGER_FIRE_AREA:
		_descriptive_text = "Larger Fire\nArea";
		break;
	case LARGER_BOMB_AREA:
		_descriptive_text = "Larger Bomb\nArea (VALUED)";
		break;
	case INCREASED_BOMB_DAMAGE:
		_descriptive_text = "Increased Bomb\nDamage (PRAISED)";
		break;
	case ABILITY_TO_RUN:
		_descriptive_text = "This upgrade literally gives you the ability to run or something I didnt check but I think so????\n(HIGHLY VALUED)";
		break;
	case ABILITY_TO_GUN:
		_descriptive_text = "Gun.\nScroll to equip";
		break;
	}
}

bool Buyable::logic(Game& g)
{
	Player& p = g._entity_handler._p;

	if (!_transaction_succeded && !_transaction_failed) {
		constexpr float threshold = 85.f;
		_show_e_sign = threshold > abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y());

		if (_show_e_sign) {
			if (g._keys_frame[SDL_SCANCODE_E])
			{
				// Possibly buy what is BUYABLE
				if (p._coins >= _cost) {
					// BUY TRANSACTION CONFIRMED AND PENDING
					p._coins -= _cost;
					_transaction_succeded = true;

					if (_onetime_index != -1) { // -1 means it was spawned part of some event, ex: boss death
						g._save._onetimes.insert({ g._level, _onetime_index });
					}

					// Variable in respect to type
					{
						using enum BUYABLE_TYPE;
						switch (_type) {
						case FASTER_FIRE_RECHARGE:
							p._fire_magic_increase *= 1.25f;
							break;
						case LARGER_FIRE_LIMIT:
							p._fire_magic_max *= 1.25;
							break;
						case INCREASED_FIRE_DAMAGE:
							p._fire_magic_damage += 2;
							break;
						case LARGER_FIRE_AREA:
							p._fire_magic_area_factor += 0.25f;
							break;
						case LARGER_BOMB_AREA:
							p._bomb_area_factor += 0.25f;
							break;
						case INCREASED_BOMB_DAMAGE:
							p._bomb_damage += 5;
							break;
						case ABILITY_TO_RUN:
							p._ability_to_run = true;
							break;
						case ABILITY_TO_GUN:
							// gun gun gun
							break;
						}
					}
				}
				else {
					_transaction_failed = true;
				}

				_show_e_sign = true;
				return false;
			}
		}
	}
	else {
		// showing text
		constexpr float threshold = 150.f;
		bool far_away = threshold < abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y());

		// only delete self if SUCCESFUL TRADE
		if (_transaction_succeded) {
			if (far_away || g._keys_frame[SDL_SCANCODE_E]) {
				delete this;
				return true;
			}
		}
		else if (_transaction_failed) {
			if (far_away || g._keys_frame[SDL_SCANCODE_E]) {
				_transaction_failed = false;
			}
		}
	}

	return false;
}
void Buyable::draw(Game& g)
{
	// Sprite and "Press E" box
	{
		const SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

		// main "sprite"
		{
			SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
			int y = rect.y;
			int x = rect.x;
			int scale = 8;
			const SDL_Rect box = { x, y, 32 * scale, 15 * scale };
			SDL_RenderCopy(g._renderer, g._textures[TEX::DialogueBox], NULL, &box);

			constexpr int text_offset = 10;
			g._cam.draw_text(g, _descriptive_text, { 0,0,0,255 }, x + text_offset, y + text_offset, 2);

			// display cost
			SDL_Rect draw_rect = { rect.x + 30,rect.y + 85,25,25 };
			SDL_RenderCopy(g._renderer, g._textures[TEX::Coin], NULL, &draw_rect);
			g._cam.draw_text(g, std::to_string(_cost).c_str(), { 0,0,0,255 }, draw_rect.x + draw_rect.w, draw_rect.y - 7, 2);
		}
		// "Press E" Text
		if (_show_e_sign) {
			constexpr int scale = 3;
			const SDL_Rect text_rect = { rect.x - 25 + (int)(10.f * sinf(((float)g._ticks) * 0.1f)), rect.y - 50, g._press_e_w_and_h[0] * scale, g._press_e_w_and_h[1] * scale };
			SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 127);
			SDL_RenderFillRect(g._renderer, &text_rect);
			SDL_RenderCopy(g._renderer, g._press_e_texture, NULL, &text_rect);
		}

		// Succeded
		if (_transaction_succeded) {
			SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
			int y = (int)((float)g._HEIGHT * 0.575f);
			int x = 267;
			int scale = 20;
			const SDL_Rect box = { x, y, 32 * scale, 15 * scale };
			SDL_RenderCopy(g._renderer, g._textures[TEX::DialogueBox], NULL, &box);

			constexpr int text_offset = 40;
			g._cam.draw_text(g, "Succesfully bought!\n:)", { 0,0,0,255 }, x + text_offset, y + text_offset, 3);
		}
		else if (_transaction_failed) {
			SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
			int y = (int)((float)g._HEIGHT * 0.575f);
			int x = 267;
			int scale = 20;
			const SDL_Rect box = { x, y, 32 * scale, 15 * scale };
			SDL_RenderCopy(g._renderer, g._textures[TEX::DialogueBox], NULL, &box);

			constexpr int text_offset = 40;
			g._cam.draw_text(g, "Transaction Failed!\nToo poor.", {0,0,0,255}, x + text_offset, y + text_offset, 3);
		}
	}
}

void Buyable::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
