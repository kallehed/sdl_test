#pragma once

#include "Player.h"
#include "QuadTree.h"

class Game;

class EntityHandler {
public:
	Player _p;
	std::vector<MovingRect*> _entities;
	std::vector<MovingRect*> _draw_entities;
	std::vector<MovingRect*> _entities_to_add;
	Quadtree<MovingRect, 4, 5> _quadtree;

	// mutable data, delegated to one Npc at a time.
	// They can change the data, but will restore it on destruction.
	char _NPC_1_TEXT[353] =
		"Hello traveller...\nYou are sure to be confused,\nbut do not worry!\0"
		"I am, The Read Bear.\nThe great helper!\0"
		"Firstly...\nAs you probably already know,\nyou can move with the arrows\0"
		"Secondly, with the mouse, you\ncan 'left click' to attack!\0"
		"and possibly also right click\nfor something else...\0"
		"Thirdly... Nothing can describe\nwhat you are about to\nexperience...";
	int _NPC_1_TOTAL_CHARS = 353;

	char _NPC_2_TEXT[109] =
		"Ah, you survived\nthe path! Great,\nyou are truly great.\0"
		"Here you have a\nbonfire which will heal\n  you! and so";
	int _NPC_2_TOTAL_CHARS = 109;

private:
	inline void do_logic(Game& g, std::vector<MovingRect*>& vec);
public:

	EntityHandler();
	void logic(Game& g);
	void draw(Game& g);
	void place_enemy(Game& g, int x, int y);
	void place_npc(Game& g, NPC_TYPE type, float x, float y);
};
