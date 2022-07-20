#include "EntityHandler.h"
#include "Game.h"

EntityHandler::EntityHandler()  : _quadtree(0, 0, 4096, 4096) {}

void EntityHandler::logic(Game& g)
{
	_p.logic(g);
	for (auto e : _entities) e->logic(g);

	//intersection logic
	/*for (EnemyBasic& e : _enemy_basics)
	{
		// player : enemies
		if (General::rect_intersection(_p, e))
		{
			float nx, ny;
			General::normalize_vector_two_points(nx, ny, _p.get_mid_x(), _p.get_mid_y(), e.get_mid_x(), e.get_mid_y());

			_p.enemy_intersection(-nx, -ny);
			e.player_intersection(nx, ny);
			break;
		}
		// shots : enemies
		for (Shot& s : _shots)
		{
			if (General::rect_intersection(s, e))
			{
				float nx, ny;
				General::normalize_vector_two_points(nx, ny, s.get_mid_x(), s.get_mid_y(), e.get_mid_x(), e.get_mid_y());
				s.hit_something(); 
				e.shot_intersection(nx, ny);
			}
		}
	}*/

	// enemy against enemy
	/*for (int i = 0; i + 1 < _enemy_basics.size(); ++i)
	{
		EnemyBasic& e1 = _enemy_basics[i];
		for (int j = i + 1; j < _enemy_basics.size(); ++j)
		{
			EnemyBasic& e2 = _enemy_basics[j];

			if (General::rect_intersection(e1, e2)) 
			{
				float nx, ny;
				General::normalize_vector_two_points(nx, ny, e1.get_mid_x(), e1.get_mid_y(), e2.get_mid_x(), e2.get_mid_y());
				e1.enemy_intersection(-nx, -ny);
				e2.enemy_intersection(nx, ny);
			}
		}
	}*/
	_quadtree.clear();

	for (auto e : _entities) {
		_quadtree.add_to_head(e);
	}
	_quadtree.add_to_head(&_p);

	_quadtree.head_do_intersection();
	
	// remove entities
	for (size_t i = _entities.size(); i-- != 0; ) // backwards loop
	{
		if (_entities[i]->end_logic(g))
		{
			delete _entities[i];
			_entities.erase(_entities.begin() + i);
		}
	}

	// mouse button press actions
	_p.possibly_shoot(g); // could append shot
}

void EntityHandler::draw(Game& g)
{
	for (auto& e : _entities) e->draw(g);
	_p.draw(g);
	_quadtree.head_draw(g);
}

void EntityHandler::place_enemy(Game& g, int x, int y)
{
	for (int _ = 1; _--;)
		_entities.push_back(new EnemyShooter((float)x + g._cam._x + _, (float)y + g._cam._y));
}