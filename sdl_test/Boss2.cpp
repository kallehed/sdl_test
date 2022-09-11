#include "Boss2.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"
#include "FireMagic.h"
#include "Particle.h"
#include "Pickupable.h"
#include "Buyable.h"
#include "EnemyBasic.h"
#include "Bomb.h"

MOVING_RECT_TYPES Boss2::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::BOSS2;
}

Boss2::Boss2(int onetime_index, float x, float y) // head
	: MovingRect(x, y, 32.f * 2.f, 39.f * 2.f, 0.01f)
{
	_onetime_index = onetime_index;
	_SHOOTING_has_shot = false;
	_RANDOM_WALK_x_dir = 0.f;
}

bool Boss2::logic(Game& g)
{
	auto& p = g._entity_handler._p;
	auto [nx, ny, dist] = General::normalize_vector_two_points_arr(mid_x(), mid_y(), p.mid_x(), p.mid_y());

	switch (_state) {
	case BOSS2_STATE::WAITING_FOR_PLAYER:
	{
		if (dist < 450.f)
		{
			// become active
			_state = BOSS2_STATE::ATTACKING;

			for (int j = 55; j <= 58; ++j) { // Block path to escape
				g._tile_handler._tiles[78][j] = TILE::BLOCK;
				g._tile_handler._texs[78][j] = TEX::CobbleStone;
			}
		}
		else {
			return false;
		}

		break;
	}

	case BOSS2_STATE::ATTACKING: // run towards player
	{
		float speed = 0.005f;
		change_x_vel(nx * speed);
		change_y_vel(ny * speed);

		_timer += g._dt;
		if (_timer >= 5000.f)
		{
			set_state_to_deciding(g);
		}

		break;
	}
	case BOSS2_STATE::DECIDING: // stay in cool distance from player
	{
		if (dist > 250) {
			float speed = 0.0025f;
			change_x_vel(nx * speed);
			change_y_vel(ny * speed);
		}
		else if (dist < 150) {
			float speed = 0.0035f;
			change_x_vel(-nx * speed);
			change_y_vel(-ny * speed);
		}

		// circle player
		float speed = 0.00175f;
		change_x_vel(ny * speed);
		change_y_vel(-nx * speed);
		
		_timer -= g._dt;
		if (_timer <= 0.f) {
			// new state
			int choice = (int)(5.f * General::randf01()); // from 0 to 4
			switch(choice) {
			case 0:
				_state = BOSS2_STATE::ATTACKING;
				_timer = 0.f;
				break;
			case 1:
				_state = BOSS2_STATE::SHOOTING;
				_SHOOTING_has_shot = true;
				_timer = 5000.f + 3000.f * General::randf01();
				break;
			case 2:
				_state = BOSS2_STATE::SPAWNING;
				_SPAWNING_has_spawned = true;
				_timer = 7000.f + 4000.f * General::randf01();
				break;
			case 3:
				_state = BOSS2_STATE::RANDOM_WALK;
				_timer = 5000.f + 2500.f * General::randf01();
				break;
			case 4:
				_state = BOSS2_STATE::BOMBING;
				_timer = 7000.f + 2000.f * General::randf01();
				break;
			default:
				std::cout << "ERROR CHOICE FOR BOSS2";
				std::cin.get();
				break;
			}
		}
		break;
	}
	case BOSS2_STATE::SHOOTING:
	{	
		// Shoot magical orbs ocasionally
		if (((int)(_timer / 1500.f)) % 2 == 0) {
			if (!_SHOOTING_has_shot) {
				_SHOOTING_has_shot = true;
				Shot* e = new Shot(this, 5, mid_x(), mid_y(), nx, ny, 0.08f, 1, TEX::MagicOrb);
				g._entity_handler._entities_to_add.emplace_back(e);
			}
		}
		else {
			if (_SHOOTING_has_shot) { // do a bit of a "jump"
				float speed = 0.05f;
				change_x_vel(-ny * speed);
				change_y_vel(nx * speed);
				
				_SHOOTING_has_shot = false;
			}	
		}

		_timer -= g._dt;
		if (_timer <= 0.f) {
			set_state_to_deciding(g);
		}
		break;
	}
	case BOSS2_STATE::SPAWNING:
	{
		if (((int)(_timer / 800.f)) % 2 == 0) {
			if (!_SPAWNING_has_spawned) {
				_SPAWNING_has_spawned = true;
				if (General::randf01() > 0.7f) {
					EnemyBasic* e = new EnemyBasic(mid_x() + General::randf01() - 0.5f, mid_y() + General::randf01() - 0.5f);
					g._entity_handler._entities_to_add.emplace_back(e);
				}
			}
		}
		else {
			if (_SPAWNING_has_spawned) {
				_SPAWNING_has_spawned = false;
				float speed = 0.1f;
				change_x_vel(-ny * speed);
				change_y_vel(nx* speed);
			}
		}

		_timer -= g._dt;
		if (_timer <= 0.f) {
			set_state_to_deciding(g);
		}
		break;
	}
	case BOSS2_STATE::RANDOM_WALK:
	{
		if ((int)(_timer / 500.f) % 2 == 0) {
			if (!_RANDOM_WALK_has_changed_dir) {
				_RANDOM_WALK_has_changed_dir = true;
				_RANDOM_WALK_x_dir = 0.5f - General::randf01();
				_RANDOM_WALK_y_dir = 0.5f - General::randf01();
			}
		}
		else {
			_RANDOM_WALK_has_changed_dir = false;
		}

		float speed = 0.01f;
		change_x_vel(_RANDOM_WALK_x_dir* speed);
		change_y_vel(_RANDOM_WALK_y_dir* speed);

		_timer -= g._dt;
		if (_timer <= 0.f) {
			set_state_to_deciding(g);
		}
		break;
	}
	case BOSS2_STATE::BOMBING:
	{
		if (((int)(_timer / 800.f)) % 2 == 0) {
			if (!_BOMBING_has_thrown_bomb) {
				_BOMBING_has_thrown_bomb = true;
				if (General::randf01() > 0.3f) {
					float speed = 0.005f * dist;
					Bomb* e = new Bomb(mid_x(), mid_y(), nx * speed, ny * speed, 10, 1.f, 2000.f);
					g._entity_handler._entities_to_add.emplace_back(e);
				}
			}
		}
		else {
			if (_BOMBING_has_thrown_bomb) {
				_BOMBING_has_thrown_bomb = false;
				float speed = 0.1f;
				change_x_vel(-nx * speed);
				change_y_vel(-ny * speed);
			}
		}

		_timer -= g._dt;
		if (_timer <= 0.f) {
			set_state_to_deciding(g);
		}
		break;
	}
	}

	// check hp situation
	if (_hp <= 0)
	{
		//heal player
		p._hp = p._max_hp;

		// UN-block path to escape
		for (int j = 55; j <= 58; ++j) {
			g._tile_handler._tiles[78][j] = TILE::VOID;
			g._tile_handler._texs[78][j] = TEX::VOID;
		}

		// spawn a boatload of coins
		{
			int coins = 100;
			for (int _ = 0; _ < coins; ++_) {
				Pickupable* e = new Pickupable(PICKUPABLE_TYPE::COIN, mid_x() + _, mid_y() + _, x_vel() / 20.f, y_vel() / 20.f);
				g._entity_handler._entities_to_add.push_back(e);
			}
		}

		// spawn gun upgrade
		{
			Buyable* e = new Buyable(-1, -100000, BUYABLE_TYPE::WIN_THE_GAME, _x, _y);
			g._entity_handler._draw_entities.emplace_back(e);
		}

		// stop from spawning again
		{
			g._save._onetimes.insert({ g._level, _onetime_index });
		}
		delete this;
		return true;
	}

	move_and_collide<true>(g);

	return false;
}

void Boss2::set_state_to_deciding(Game& g)
{
	_state = BOSS2_STATE::DECIDING;
	_timer = 4000.f + General::randf01() * 3000.f;
}


void Boss2::draw(Game& g)
{
	SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

	draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 25, { 0,0,0,67 });

	TEX::TEX tex = ((g._ticks / 30)%2 == 0) ? TEX::GreenSlime1 : TEX::GreenSlime12;
	SDL_RenderCopy(g._renderer, g._textures[tex], NULL, &rect);
}

void Boss2::intersection(Game& g, float nx, float ny, MovingRect* e)
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
					Particle* e = new Particle(mid_x(), mid_y(), x_vel * General::randf01(), y_vel * General::randf01(), { 0, 200, 255, 255 }, 40);
					g._entity_handler._particles.emplace_back(e);
				}
			}
			else {
				float x_vel = nx * 3.f;
				float y_vel = ny * 3.f;
				int total_particles = 50;
				for (int i = 0; i < total_particles; ++i) {
					Particle* e = new Particle(mid_x(), mid_y(), x_vel * (0.25f + 0.75f * General::randf01()), y_vel * (0.25f + 0.75f * General::randf01()), { 0, 200, 255, 255 }, 100);
					g._entity_handler._particles.emplace_back(e);
				}
			}
		}
	}
}


void Boss2::take_damage(Game& g, int damage)
{
	_hp -= damage;
}
