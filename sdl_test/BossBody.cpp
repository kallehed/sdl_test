#include "BossBody.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"
#include "FireMagic.h"
#include "Particle.h"
#include "Pickupable.h"
#include "Buyable.h"

MOVING_RECT_TYPES BossBody::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::BOSS;
}

int BossBody::_onetime_index = -1;

BossBody::BossBody(int onetime_index, float x, float y) // head
	: MovingRect(x, y, 50.f, 50.f, 0.01f),
	_more_to_add(_largest_more_to_add),
	_next(nullptr)
{
	_onetime_index = onetime_index;
}

BossBody::BossBody(float x, float y, int more_to_add, BossBody* next) // body
	: MovingRect(x, y, 50.f, 50.f, 0.01f),
	_more_to_add(more_to_add),
	_next(next)
{
}

bool BossBody::logic(Game& g)
{
	auto& p = g._entity_handler._p;

	if (_more_to_add == _largest_more_to_add) // code only run in waiting for player to arrive
	{
		_hp = _max_hp;
		 // Head, will start the boss when player is near
		if (600.f > abs(p.mid_x() - mid_x()) + abs(p.mid_y() - mid_y()))
		{
			// start boss

			// block path to escape
			for (int i = 48; i <= 51; ++i) {
				g._tile_handler._tiles[i][51] = TILE::BLOCK;
				g._tile_handler._texs[i][51] = TEX::CobbleStone;
			}
		}
		else {
			return false; // do nothing
		}
	}

	if (_more_to_add > 0)
	{
		BossBody* e = new BossBody(_x + 20, _y, _more_to_add - 1, this);
		_back = e;
		g._entity_handler._entities_to_add.emplace_back(e);

		_more_to_add = 0;
	}

	// check hp situation
	if (_hp <= 0)
	{
		if (_back == nullptr && _next == nullptr)
		{
			// last one to die.

			// UN-block path to escape
			for (int i = 48; i <= 51; ++i) {
				g._tile_handler._tiles[i][51] = TILE::VOID;
				g._tile_handler._texs[i][51] = TEX::VOID;
			}

			// spawn a boatload of coins
			{
				int coins = 50;
				for (int _ = 0; _ < coins; ++_) {
					Pickupable* e = new Pickupable(PICKUPABLE_TYPE::COIN, mid_x() + _, mid_y() + _, x_vel() / 20.f, y_vel() / 20.f);
					g._entity_handler._entities_to_add.push_back(e);
				}
			}

			// spawn gun upgrade
			{
				Buyable* e = new Buyable(-1, 0, BUYABLE_TYPE::ABILITY_TO_GUN, _x, _y);
				g._entity_handler._draw_entities.emplace_back(e);
			}

			// stop from spawning again
			{
				g._save._onetimes.insert({ g._level, _onetime_index });
			}
		}
		else
		{ // fix hole in linked list
			if (_back != nullptr) { // remove self from linked list
				_back->_next = _next;
			}
			if (_next != nullptr) {
				_next->_back = _back;
			}
		}

		delete this;
		return true;
	}


	// speedy mode logic

	if (_speedy_mode) {
		if (_timer > 0.f) {
			_timer -= g._dt;
		}
		else {
			// turn off speedy mode
			_speedy_mode = false;
			_timer = -420.f;
		}
	}
	else {

		// head logic for activating timer for speedy mode
		if (_next == nullptr)
		{
			if (_timer == -420.f) { // start timer
				_timer = 2000.f + 12000.f * ((float)_hp / _max_hp);
			}
			else if (_timer > 0.f) {
				_timer -= g._dt;
			}
			else {
				// activate speedy mode
				activate_speedy_mode(4000.f + 100.f*(_max_hp - _hp), 0);
			}
		}
	}
	

	MovingRect* target = (_next == nullptr) ?
		static_cast<MovingRect*>(&g._entity_handler._p) : static_cast<MovingRect*>(_next);

	// turn towards target
	{
		float r = atan2(target->mid_y() - mid_y(), target->mid_x() - mid_x()); // SECOND HALF IS NEGATIVE FOR SOME REASON???

		if (r < 0.f) { r += General::tau(); } // BECAUSE OF THAT, THIS

		float r_cor = General::normalize_radian(r - _radians_turned);

		constexpr float turn = 0.024f;

		if (r_cor > General::pi()) {
			_radians_turned -= turn;
		}
		else {
			_radians_turned += turn;
		}

	}
	_radians_turned = General::normalize_radian(_radians_turned);

	_x_dir = cosf(_radians_turned);
	_y_dir = sinf(_radians_turned);

	float speed = !_speedy_mode  ? 0.002f : 0.0034f;
	change_x_vel(_x_dir * speed);
	change_y_vel(_y_dir* speed);

	move_and_collide<true>(g);

	return false;
}

void BossBody::activate_speedy_mode(float time, Uint8 color)
{
	_speedy_mode = true;
	_timer = time;
	_color = color;
	if (_back != nullptr)
	{
		_back->activate_speedy_mode(time, color + (220/_largest_more_to_add));
	}
}

void BossBody::draw(Game& g)
{
	SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

	draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 25, { 0,0,0,67 });

	TEX::TEX tex = TEX::RedHumanBackwards;

	if (_next != nullptr)
	{
		tex = TEX::BossBody;
	}
	else {
		tex = (((int)_timer / (_speedy_mode?150:500)) % 2 == 0) ? TEX::BossHead : TEX::BossHeadBite;
	}

	double rot = (double)General::radians_to_degrees(_radians_turned);

	if (_speedy_mode) {
		SDL_SetTextureColorMod(g._textures[tex], 255, _color, _color);
	}
	else {
		SDL_SetTextureColorMod(g._textures[tex], 255, 255, 255);
	}

	SDL_RenderCopyEx(g._renderer, g._textures[tex], NULL, &rect, rot, NULL, (SDL_RendererFlip)(SDL_FLIP_VERTICAL|SDL_FLIP_HORIZONTAL));

}

void BossBody::intersection(Game& g, float nx, float ny, MovingRect* e)
{
	int damage = 0;
	float bounce_acc = 0.f;

	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::BOSS:
	{
		bounce_acc = 0.0075f;
		break;
	}
	case MOVING_RECT_TYPES::SHOT:
	{
		if (((Shot*)e)->_owner == this) { // can't shoot self(not instantly anyway)
			break;
		}
		bounce_acc = 0.05f;

		damage = 5;
		break;
	}
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		bounce_acc = 0.1f;

		damage = ((Explosion*)e)->_damage;
		break;
	}
	case MOVING_RECT_TYPES::FIRE_MAGIC:
	{
		bounce_acc = 0.05f;

		damage = ((FireMagic*)e)->_damage;
		break;
	}
	}

	if (bounce_acc != 0.f) {
		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
	}

	if (damage > 0) {
		this->take_damage(g, damage);

		// shake screen
		g._cam.shake(g, 1.3f, 10.f);

		// particles
		{
			if (_hp > 0) {
				float x_vel = nx;
				float y_vel = ny;
				int total_particles = damage;
				for (int i = 0; i < total_particles; ++i) {
					Particle* e = new Particle(mid_x(), mid_y(), x_vel * General::randf01(), y_vel * General::randf01(), { 0, 200, 255, 255 }, 20);
					g._entity_handler._particles.emplace_back(e);
				}
			}
			else {
				float x_vel = nx * 3.f;
				float y_vel = ny * 3.f;
				int total_particles = 50;
				for (int i = 0; i < total_particles; ++i) {
					Particle* e = new Particle(mid_x(), mid_y(), x_vel * (0.25f + 0.75f * General::randf01()), y_vel * (0.25f + 0.75f * General::randf01()), { 0, 200, 255, 255 }, 50);
					g._entity_handler._particles.emplace_back(e);
				}
			}
		}
	}
}


void BossBody::take_damage(Game& g, int damage)
{
	_hp -= damage;
}
