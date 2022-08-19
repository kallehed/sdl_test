#pragma once

#include "Player.h"
#include "QuadTree.h"

class Game;

class EntityHandler {
public:
	Player _p;
	std::vector<MovingRect*> _entities;
	std::vector<MovingRect*> _draw_entities;
	std::vector<MovingRect*> _particles;
	std::vector<MovingRect*> _entities_to_add;
	Quadtree<MovingRect, 4, 5> _quadtree;

	// mutable data, delegated to one Npc at a time.
	// They can change the data, but will restore it on destruction.
	char _NPC_1_TEXT[355] =
		"Hello traveller...   \nYou are sure to be confused,\nbut do not worry!\0"
		"I am, The Read Bear.  \nThe great helper!\0"
		"Firstly...   \nAs you probably already know,   \nyou can move with the arrows.\0"
		"Secondly, with the mouse, you\ncan 'left click' to attack!\0"
		"and possibly also right click\nfor something else...\0"
		"Thirdly...      \nI have nothing more to say...\0"     
		"Good luck?";
	int _NPC_1_TOTAL_CHARS = 355;

	char _NPC_2_TEXT[245] =
		"Ah, you survived the path!    \nGreat, you are truly strong\nin many ways.\0"
		"Still, it might be wise to\nheal, and restore your health.    \nProbably for the best.\0"
		"Here you have a bonfire, which\nwill heal all your worries\naway!\0"
		"Go ahead and use it...";
	int _NPC_2_TOTAL_CHARS = 245;

	char _NPC_GNOME_TEXT[107] =
		"Welcome to the Gnome Fire\nShop!\0"
		"Full of upgrades waiting\nto be absorbed by some\ngreat adventurer!\0"
		"Enjoy...";
	int _NPC_GNOME_TOTAL_CHARS = 107;

	char _NPC_GREEN_BOMB_GUY[75] =
		"HI PERSON,\0"
		"THIS IS THE BOMB SHOP,\0"
		"WHERE YOU CAN BUY BOMB\nUPGRADES\0"
		"BUY THEM";
	int _NPC_GREEN_BOMB_GUY_TOTAL_CHARS = 75;

	char _NPC_3_TEXT[138] = 
		"Hi again\0"
		"It seems you still live?\0"
		"How ...     \nWonderful.\0"
		"Anyway...\0"
		"I heard you can deflect\nmagical orbs with fire...\0"
		"Just a         \ntip";
	int _NPC_3_TOTAL_CHARS = 138;

	char _NPC_SUS_SELLER_TEXT[218] =
		"Hey...\0"
		"kid...\0"
		"you could have that upgrade\nbelow me for 1 spare change...\0"
		"It's quite...    \nNecessary?..\0"
		"Though,    \nyou won't approve of what\ni'm going to spend the coins\non...\0"
		"(The upgrade can be used with\n'SHIFT')..";
	int _NPC_SUS_SELLER_TEXT_TOTAL_CHARS = 218;

	char _NPC_SUS_SELLER_TEXT2[48] =
		"yo...\0thanks for the coin...\0hehe...           ";
	int _NPC_SUS_SELLER_TEXT2_TOTAL_CHARS = 48;

private:
	inline void do_logic(Game& g, std::vector<MovingRect*>& vec);
public:

	EntityHandler();
	void logic(Game& g);
	void draw(Game& g);
	void place_enemy(Game& g, int x, int y);
	void place_npc(Game& g, NPC_TYPE type, float x, float y);
};
