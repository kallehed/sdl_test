#include "Enemy.h"
#include "Game.h"
#include "General.h"
#include "Pickupable.h"
#include "Particle.h"
#include "Explosion.h"
#include "FireMagic.h"
#include "Shot.h"
#include "AStarNode.h"

Enemy::Enemy(float x, float y,float w, float h, int max_hp, float activation_radius, float deactivation_radius, float active_time, float active_basic_speed, float idle_still_time, float idle_walk_time, float get_away_distance, float get_closer_distance, float idle_speed)
	: MovingRect(x, y, w, h, 0.01f),
	_max_hp(max_hp), _hp(max_hp), _activation_radius_squared(activation_radius*activation_radius),
	_deactivation_radius_squared(deactivation_radius*deactivation_radius), _active_time(active_time),
	_active_basic_speed(active_basic_speed),
	_idle_still_time(idle_still_time), _idle_walk_time(idle_walk_time),
	_get_away_distance_squared(powf(get_away_distance, 2.f)), _get_closer_distance_squared(powf(get_closer_distance, 2.f)),
	_idle_speed(idle_speed)
{
	set_active(false);
}

bool Enemy::logic(Game& g)
{
	// handle phases
	if (!_active) // idle phase
	{
		idle_logic(g);

		// potentially switch phase
		if (in_radius_squared_of_player(g, _activation_radius_squared))
		{
			set_active(true);
		}
	}
	else { // active phase
		
		active_logic(g);

		_timer += g._dt;
		if (_timer > _active_time) // return to idle phase
		{ 
			set_active(false);
		}
		else if (in_radius_squared_of_player(g, _deactivation_radius_squared)) // keep being in attack phase
		{ 
			_timer = 0.f;
		}
	}

	move_and_collide<true>(g);

	// particles
	{
		if (abs(x_vel()) + abs(y_vel()) >= 0.05f && g._ticks % (20 + rand() % 11) == 0) {
			g._entity_handler._particles.emplace_back(new Particle(mid_x(), mid_y(), 0.f, 0.f, { 0, 200, 255, 255 }));
		}
	}

	// kill self?
	if (_hp < 1) {
		// spawn coins?
		{
			int coins = 1 + rand() % 2;
			for (int _ = 0; _ < coins; ++_) {
				Pickupable* e = new Pickupable(PICKUPABLE_TYPE::COIN, mid_x() + _, mid_y() + _, x_vel() / 20.f, y_vel() / 20.f);
				g._entity_handler._entities_to_add.push_back(e);
			}
		}

		delete this;
		return true;
	}
	return false;
}

void Enemy::idle_logic(Game& g)
{
	_timer += g._dt;
	if (!_idle_state) // wait for a moment still
	{
		if (_timer > _idle_still_time)
		{
			_idle_state = true;
			_timer = 1000.f * General::randf01();

			float random_radians = General::tau() * General::randf01();
			_idle_x_dir = cosf(random_radians);
			_idle_y_dir = sinf(random_radians);
		}
	}
	else // walk
	{
		change_x_vel(_idle_speed * _idle_x_dir);
		change_y_vel(_idle_speed * _idle_y_dir);
		if (_timer > _idle_walk_time) {
			_idle_state = false;
			_timer = 0.f;
		}
	}
}

void Enemy::stay_in_range_of_player(Game& g)
{
	// check if distance to player is too far away => go closer
	float distance_to_player_squared;
	{
		float dx = mid_x() - g._entity_handler._p.mid_x();
		float dy = mid_y() - g._entity_handler._p.mid_y();
		distance_to_player_squared = dx * dx + dy * dy;
	}
	if (distance_to_player_squared > _get_closer_distance_squared)
	{
		// too far away
		if (g._tile_handler.is_path_clear(g, x(), y(), g._entity_handler._p.x(), g._entity_handler._p.y()))
		{
			go_towards_player(g, _active_basic_speed);
			_walk_path.clear(); // clear path
		}
		else
		{
			// walk in path
			if (_walk_path.empty() || _path_progress >= _walk_path.size())
			{
				new_walk_path(g);
				if (_walk_path.empty())
				{
					go_towards_player(g, _active_basic_speed);
				}
			}
			else // move through A* path
			{
				// left upper point of tile
				float tile_x = _walk_path[_path_progress][1] * g._cam._fgrid;
				float tile_y = _walk_path[_path_progress][0] * g._cam._fgrid;
				// where the (middle point of this rect) should go
				float dst_x = tile_x + g._cam._fgrid / 2.f;
				float dst_y = tile_y + g._cam._fgrid / 2.f;
				go_towards(dst_x, dst_y, _active_basic_speed);

				// completely inside tile => go to next
				if (x() > tile_x && y() > tile_y &&
					x() + w() < tile_x + g._cam._grid && y() + h() < tile_y + g._cam._grid)
				{
					_path_progress += 1;
				}
			}
		}
	}
	else if (distance_to_player_squared < _get_away_distance_squared)
	{
		// get away
		go_towards_player(g, (-1.f) * _active_basic_speed);
	}
	else {
		// best bounds, stay still?
	}
}

void Enemy::set_active(bool active)
{
	_active = active;
	_timer = 0.f;
}


void Enemy::go_towards_player(Game& g, float speed)
{
	this->go_towards(g._entity_handler._p.mid_x(), g._entity_handler._p.mid_y(), speed);
}

bool Enemy::in_radius_squared_of_player(Game& g, float radius_squared)
{ 
	float dx = mid_x() - g._entity_handler._p.mid_x();
	float dy = mid_y() - g._entity_handler._p.mid_y();
	return dx * dx + dy * dy <= radius_squared;
}

void Enemy::take_damage(Game& g, int damage) {
	_hp -= damage;
	_hurt_timer = 150.f;
}

void Enemy::new_walk_path(Game& g) // clear
{
	_path_progress = 1;
	_walk_path = AStarNode::generate_walk_path(g, mid_x(), mid_y(), g._entity_handler._p.mid_x(), g._entity_handler._p.mid_y());
}

void Enemy::intersection(Game& g, float nx, float ny, MovingRect* e)
{
	int damage = 0;
	float bounce_acc = 0.f;
	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::SHOT:
	{
		if (((Shot*)e)->_owner == this) { // can't shoot self(not instantly anyway)
			break;
		}
		bounce_acc = 0.05f;
		
		damage = ((Shot*)e)->_damage;
		break;
	}
	case MOVING_RECT_TYPES::ENEMY:
	{
		bounce_acc = 0.005f;
		break;
	}
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		bounce_acc = 0.1f;

		damage =  ((Explosion*)e)->_damage;
		break;
	}
	case MOVING_RECT_TYPES::FIRE_MAGIC:
	{
		bounce_acc = 0.05f;

		damage =  ((FireMagic*)e)->_damage;
		break;
	}
	}

	if (bounce_acc != 0.f) {
		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
	}

	if (damage > 0) {
		take_damage(g, damage);

		// shake screen
		g._cam.shake(g, 1.3f, 10.f);

		set_active(true); // become active (aggressive)

		// particles
		{
			if (_hp > 0) {
				float x_vel = nx;
				float y_vel = ny;
				int total_particles = damage;
				for (int i = 0; i < total_particles; ++i) {
					Particle* e = new Particle(mid_x(), mid_y(), x_vel * General::randf01(), y_vel * General::randf01(), { 0, 200, 255, 255 });
					g._entity_handler._particles.emplace_back(e);
				}
			}
			else {
				float x_vel = nx * 3.f;
				float y_vel = ny * 3.f;
				int total_particles = 50;
				for (int i = 0; i < total_particles; ++i) {
					Particle* e = new Particle(mid_x(), mid_y(), x_vel * (0.25f + 0.75f*General::randf01()), y_vel * (0.25f + 0.75f * General::randf01()), { 0, 200, 255, 255 });
					g._entity_handler._particles.emplace_back(e);
				}
			}
		}
	}
}
