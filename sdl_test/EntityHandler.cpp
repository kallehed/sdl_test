#include "EntityHandler.h"
#include "Game.h"
#include "EnemyShooter.h"
#include "Npc.h"

EntityHandler::EntityHandler()  : _quadtree(0, 0, 5632, 5632) {}

inline void EntityHandler::do_logic(Game& g, std::vector<MovingRect*>& vec)
{
	for (int i = ((int)vec.size()) - 1; i > -1; --i) {
		if (vec[i]->logic(g)) // delete if entity returns true
		{
			vec.erase(vec.begin() + i);
		}
	}
}

void EntityHandler::logic(Game& g)
{
	this->do_logic(g, _particles);

	_p.logic(g); // player could add particles

	// _draw_entities first, cuz _entitites could add to _draw_entitites

	this->do_logic(g, _draw_entities);

	this->do_logic(g, _entities);

	// quadtree stuff, intersection!
	_quadtree.clear();

	for (auto e : _entities) {
		e->insert_to_intersect(*this);
	}
	_p.insert_to_intersect(*this);

	_quadtree.head_do_intersection(g);
	
	// add entities
	for (auto e : _entities_to_add)
	{
		_entities.push_back(e);
	}
	_entities_to_add.clear();
}

void EntityHandler::draw(Game& g)
{
	for (auto& e : _particles) e->draw(g);
	for (auto& e : _entities) e->draw(g);
	for (auto& e : _draw_entities) e->draw(g);
	_p.draw(g);
	if constexpr (DEV::DEV) _quadtree.head_draw(g);
}

void EntityHandler::place_enemy(Game& g, int x, int y)
{
	for (int _ = 1; _--;)
		_entities.push_back(new EnemyShooter((float)x + g._cam._x + _, (float)y + g._cam._y));
}

void EntityHandler::place_npc(Game& g, NPC_TYPE type, float x, float y)
{
	_draw_entities.push_back(new Npc(g, type, x, y));
}
