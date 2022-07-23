#include "EntityHandler.h"
#include "Game.h"
#include "EnemyShooter.h"

EntityHandler::EntityHandler()  : _quadtree(0, 0, 4096, 4096) {}

inline void EntityHandler::do_logic(Game& g, std::vector<MovingRect*>& vec)
{
	for (int i = 0; i < vec.size();)
	{
		if (vec[i]->logic(g)) // delete if entity returns true
		{
			vec.erase(vec.begin() + i);
		}
		else {
			++i;
		}
	}
}

void EntityHandler::logic(Game& g)
{
	_p.logic(g);

	// _draw_entities first, cuz _entitites could add to _draw_entitites
	
	this->do_logic(g, _draw_entities);

	this->do_logic(g, _entities);

	// quadtree stuff, intersection!
	_quadtree.clear();

	for (auto e : _entities) {
		_quadtree.add_to_head(e);
	}
	_quadtree.add_to_head(&_p);

	_quadtree.head_do_intersection();
	
	// add entities
	for (auto e : _entities_to_add)
	{
		_entities.push_back(e);
	}
	_entities_to_add.clear();
}

void EntityHandler::draw(Game& g)
{
	for (auto& e : _entities) e->draw(g);
	for (auto& e : _draw_entities) e->draw(g);
	_p.draw(g);
	_quadtree.head_draw(g);	
}

void EntityHandler::place_enemy(Game& g, int x, int y)
{
	for (int _ = 1; _--;)
		_entities.push_back(new EnemyShooter((float)x + g._cam._x + _, (float)y + g._cam._y));
}