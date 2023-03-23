#include "Player.h"
#include "Game.h"
#include "Bomb.h"
#include "Shot.h"
#include "FireMagic.h"
#include "Pickupable.h"
#include "Explosion.h"
#include "Particle.h"
#include "Enemy.h"
#include "BossBody.h"
#include "Boss2.h"

MOVING_RECT_TYPES Player::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::PLAYER;
}

Player::Player() : MovingRect(0.f, 0.f, 40.f, 60.f, 0.009f) {
}

bool Player::logic(Game& g)
{
	_took_damage_this_frame = false; // reset every frame

	// death stuff
	if (_hp <= 0 || !_alive) {
		if (_alive) {
			// moment of death code
			_alive = false;
			_death_timer = 2250.f;
			// particles?
			{
				int total_particles = 150;
				float x = mid_x();
				float y = mid_y();

				for (int i = 0; i < total_particles; ++i) {
					float x_vel = (General::randf01() - 0.5f)*8.f;
					float y_vel = (General::randf01() - 0.5f)*8.f;
					Particle* e = new Particle(x, y, x_vel, y_vel, { 255,0,0,255 });
					g._entity_handler._particles.emplace_back(e);
				}
			}
		}
		if (_death_timer <= 0.f) {
			// respanw!
			_respawn = true;
		}
		_death_timer -= g._dt;
		return false;
	}

	// moving based on arrows/WASD
	{
		float acc = 0.0023f;

		if (_ability_to_run) {
			if (!_using_run) {
				if (_run_current > 0.f && g._keys_frame[SDL_SCANCODE_LSHIFT]) {
					// active running
					_using_run = true;
				}
				else {
					_run_current += g._dt * _run_recharge;
					_run_current = std::min(_run_current, _run_max);
				}
			}
			else {
				// run is active!
				if (g._keys_down[SDL_SCANCODE_LSHIFT] && _run_current > 0.f) {
					// INCREASE SPEED !!!!!!!!!!!
					acc *= _run_speed_boost;
					_run_current -= g._dt * _run_cost;
				}
				else {
					// STOP RUNNING!!!
					_using_run = false;
				}
			}
		}
		// change velocities according to keys pressed
		auto& keys = g._keys_down;
		if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
			change_x_vel(acc);
			_right = true;
		}
		if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
			change_x_vel(-acc);
			_right = false;
		}
		if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
			change_y_vel(acc);
			_forward = true;
		}
		if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
			change_y_vel(-acc);
			_forward = false;
		}
//#define __ANDROID__

#ifdef __ANDROID__
		if (g._finger_down) {
			int m_xi, m_yi;
			Uint32 buttons = g.getMouseState(&m_xi, &m_yi);
			float m_x = m_xi / (float)g._WIDTH, m_y = m_yi / (float)g._HEIGHT;
			if (m_x <= 0.25f && m_y >= 0.75f) {
				change_x_vel(((m_x > 0.125f) ? (acc) : (-acc)));
				change_y_vel(((m_y > 0.865f) ? (acc) : (-acc)));
			}
		}
#endif
	}

	// let MovingRect handle the rest
	move_and_collide<true>(g);

	// invincibility
	{
		if (_invi_timer > 0.f) {
			_invi_timer -= g._dt;
		}
	}

	// particles when walking
	{
		int probability = (!_using_run) ? 11 : 3;
		if (abs(x_vel()) + abs(y_vel()) >= 0.05f && g._ticks % ((long long)probability + rand()%11) == 0) {
			g._entity_handler._particles.emplace_back(new Particle(mid_x(), mid_y(), -x_vel()/4.f, -y_vel()/4.f, _particle_color));
		}
	}

	// fire magic logic
	{
		_fire_magic_current += _fire_magic_increase * g._dt;
		_fire_magic_current = std::min(_fire_magic_current, _fire_magic_max);
	}

	// use weapons(both left and right)
	{
		int m_x, m_y;
		Uint32 buttons = g.getMouseState(&m_x, &m_y);

		// left mouse button weapons
		{
			_left_timer += g._dt;

			// switch left weapon
			_left_weapon = (L_WEAPON::_)General::mod(_left_weapon + g._mouse_scroll, L_WEAPON::TOTAL);

			// scroll further until finding available weapon
			while (_have_l_weapon[_left_weapon] == false)
			{ 
				_left_weapon = (L_WEAPON::_)General::mod(_left_weapon + 1, L_WEAPON::TOTAL);
			}
			
			switch (_left_weapon) {
			case L_WEAPON::FIRE_MAGIC:
			{
				if (g._mouse_btn_pressed_this_frame[0]) // left
				{
					if (_left_timer > 150.f && _fire_magic_current >= _fire_magic_cost)
					{
						_left_timer = 0.f;
						_fire_magic_current -= _fire_magic_cost;

						float nx, ny;
						General::normalize_vector_two_points(nx, ny, g._cam.convert_x(mid_x()), g._cam.convert_y(mid_y()), (float)m_x, (float)m_y);

						float displacement = 40.f;

						float e_x = mid_x() + nx * displacement;
						float e_y = mid_y()+ ny * displacement;

						FireMagic* e = new FireMagic(this, e_x, e_y, _fire_magic_damage, _fire_magic_area_factor);
						g._entity_handler._entities_to_add.push_back(e);

						g.play_sound(SOUND::Fire);
					}
				}
				break;
			}
			case L_WEAPON::GUN:
			{
				// possibly shoot
				
				if (_shots > 0 && _left_timer > _shoot_time)
				{
					if ((buttons & SDL_BUTTON_LMASK) != 0) // left
					//if (g._mouse_btn_pressed_this_frame[0]) // left mouse btn
					{
						_left_timer = 0.f;
						--_shots;

						// shoot projectile "Shot"
						float nx, ny;
						General::normalize_vector_two_points(nx, ny, g._cam.convert_x(mid_x()), g._cam.convert_y(mid_y()), (float)m_x, (float)m_y);
						g._entity_handler._entities_to_add.push_back(new Shot(this, _shot_damage, mid_x(), mid_y(), nx, ny, _shot_speed, _shot_lives ));
					
						g.play_sound(SOUND::Shoot);
					}
				}
				break;
			}
			}
		}

		// right weapons
		{
			// throw bomb
			if (!_charging_bomb_throw) // "not doing anything bomby" state
			{
				// possibly start charging bomb throw?
				_bomb_recharge_timer -= g._dt;
				if (_bomb_recharge_timer <= 0.f && _bombs > 0 && g._mouse_btn_pressed_this_frame[2]) // right mouse => start charging
				{
					_charging_bomb_throw = true;
				}
			}
			else
			{	// "charging bomb throw" state

				// check if right mouse button has been let go
				if ((buttons & SDL_BUTTON_RMASK) == 0)
				{
					float bomb_speed = 1.5f * _bomb_throw_charge / _bomb_throw_max_charge;
					float nx, ny;
					General::normalize_vector_two_points(nx, ny, g._cam.convert_x(mid_x()), g._cam.convert_y(mid_y()), (float)m_x, (float)m_y);

					float x_vel = nx * bomb_speed;
					float y_vel = ny * bomb_speed;

					g._entity_handler._entities_to_add.push_back(new Bomb(g, mid_x(), mid_y(), x_vel, y_vel, _bomb_damage, _bomb_area_factor));

					// reset
					_charging_bomb_throw = false;
					_bomb_throw_charge = 0.f;
					_bomb_recharge_timer = _bomb_recharge_time;
					--_bombs;
				}
				else
				{
					_bomb_throw_charge += g._dt * (float)_bomb_charge_direction;

					// change charging direction?
					if (_bomb_throw_charge >= _bomb_throw_max_charge)
					{
						_bomb_charge_direction = -1;
					}
					else if (_bomb_throw_charge <= 0.f)
					{
						_bomb_charge_direction = 1;
					}
				}
			}
		}
	}
	return false;
}


void Player::draw(Game& g)
{
	if (!_alive) { return; }

	SDL_Rect rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()),(int)w(),(int)h() };

	// draw shadow
	draw_circle(g._renderer, rect.x + _w/2, rect.y + _h*0.9f, 20, {0,0,0,67});

	SDL_RendererFlip flip = _right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
	// first choice texture
	SDL_Texture* tex = _forward ? g._textures[TEX::RedHuman] : g._textures[TEX::RedHumanBackwards];

	// walk animation, if walking
	constexpr float walk_bound = 0.02f;
	if (std::abs(x_vel()) > walk_bound || std::abs(y_vel()) > walk_bound) {
		// should walk

		if ((((int)_walk_animation_timer) / 200) % 2 == 0) {
			tex = _forward ? g._textures[TEX::RedHumanWalk1] : g._textures[TEX::RedHumanBackwardsWalk1];
		}
		else {
			tex = _forward ? g._textures[TEX::RedHumanWalk2] : g._textures[TEX::RedHumanBackwardsWalk2];
		}
		_walk_animation_timer += g._dt;
	}
	else {
		_walk_animation_timer = 0.f;
	}
	

	if (_invi_timer > 0.f) {
		if ((((int)_invi_timer) / 50) % 2 == 0) {
			SDL_SetTextureColorMod(tex, 100, 100, 100);
		}
		else {
			SDL_SetTextureColorMod(tex, 200, 200, 200);
		}
	}
	else {
		SDL_SetTextureColorMod(tex, 255, 255, 255);
	}

	SDL_RenderCopyEx(g._renderer, tex, NULL, &rect, NULL, NULL, flip);

	// draw black flash when hit
	if (_took_damage_this_frame) {
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 126);
		const SDL_Rect full_screen_rect = { 0,0,g._WIDTH, g._HEIGHT };
		SDL_RenderFillRect(g._renderer, &full_screen_rect);
	}
}

void Player::take_damage(Game& g, int damage)
{
	if (_invi_timer <= 0.f) {
		_hp -= damage;
		_hp = std::max(0, _hp);
		_invi_timer = _invi_time;

		g._cam.shake(g, 1.5f, 100.f);
		g._slow_motion_factor = 0.5f;

		_took_damage_this_frame = true;

		g.play_sound(SOUND::PlayerHit);
	}
}

void Player::intersection(Game& g, float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type())
	{
	case MOVING_RECT_TYPES::ENEMY: // handle bumping into enemy
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage(g, ((Enemy*)e)->_damage);
		break;
	}
	case MOVING_RECT_TYPES::BOSS:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage(g, ((BossBody*)e)->_damage);
		break;
	}
	case MOVING_RECT_TYPES::BOSS2:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage(g, ((Boss2*)e)->_damage);
		break;
	}
	case MOVING_RECT_TYPES::SHOT:
	{
		if (this == ((Shot*)e)->_owner) { // don't kill self
			break;
		}

		float bounce_acc = 0.05f;
		
		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage(g, ((Shot*)e)->_damage);
		break;
	}
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		float bounce_acc = 0.1f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage(g, static_cast<Explosion*>(e)->_damage);
		break;
	}
	case MOVING_RECT_TYPES::PICKUPABLE:
	{
		Pickupable* p = static_cast<Pickupable*>(e);
		if (p->_type == PICKUPABLE_TYPE::COIN) {
			g.play_sound(SOUND::Coin);
			_coins += 1;
		}
		else if (p->_type == PICKUPABLE_TYPE::SHOT) {
			// 1 to 3 possible gain
			_shots += _shot_base_gain + (rand() % 4);
			g.play_sound(SOUND::GetItem);
		}
		else if (p->_type == PICKUPABLE_TYPE::HEART) {
			// constant gain
			_hp += _heart_hp_gain;
			_hp = std::min(_max_hp, _hp);
			g.play_sound(SOUND::Heal);
		}
		else if (p->_type == PICKUPABLE_TYPE::BOMB) {
			_bombs += 2 + (rand() % 2);
			g.play_sound(SOUND::GetItem);
		}
		break;
	}
	}
}
