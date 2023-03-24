#pragma once

#include "Player.h"
#include "Quadtree.h"

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
#ifndef __ANDROID__
	char _NPC_1_TEXT[284] = 
		"Oh\0Right\0Hi, do not fret,\nfor I will help you!\0"
		"Firstly...   \nAs you probably already know,   \nyou can move with the arrows.\0"
		"Secondly, you can 'left click'\nto attack with fire!\0"
		"Maybe also something with\nyour 'right click'...\0"
		"Thirdly...      \nI have nothing more to say...\0"
		"Good luck?";
	int _NPC_1_TOTAL_CHARS = 284;
#else
	char _NPC_1_TEXT[3720] =
		"Oh\0Right\0Hi, do not fret,\nfor I will help you!\0"
		"Ahhh, very interesting!\0It seems you are an\nAndroid user?\0"
		"Well...   \nAs you probably already know,   \nyou can move by toucing the\0"
		"bottom left corner.\0"
		"Secondly, you can touch the\nscreen anywhere else to\nattack with fire!\0"
		"Once you get bombs\nyou will also be able\nto triple touch and quadruple\0touch to throw bombs!\0"
		"Thirdly...      \nI have nothing more to say...\0"
		"Except wait,     \nin the VERY far future,   \nyou will be able to\0switch weapons!\0"
		"That will be\nachieved by placing FIVE\nfingers at once on the screen\0"
		"Anyway...   \nGood luck?";
	int _NPC_1_TOTAL_CHARS = 3720;
#endif

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

	char _NPC_3_TEXT[135] = 
		"Hi again\0"
		"It seems you still live?\0"
		"How ...     \nWonderful.\0"
		"Anyway\0"
		"I heard you can deflect\nmagical orbs with fire...\0"
		"Just a         \ntip";
	int _NPC_3_TOTAL_CHARS = 135;

	char _NPC_4_TEXT[436] =
		"*snacking*\0"
		"*fumbling*\0"
		"*resting*\0"
		"*snoring*\0"
		"*snickering*                           \nGASP! YOU AGAIN!\nDISTURBING MY SLEEP!\0"
		"How ...           \nWell timed.\0"
		"It seems you still\ncontinue to exist?\0"
		"I would advice to continue\0"
		"But, do threat, for there is a\ngreat danger forthcoming...\0"
		"A dangerous monstrosity of\na monster...\0"
		"Only a brave warrior like\nyourself could be thought\nto challenge it!\0"
		"So do go ahead, but\nwith a great deal of anxiety...";
	int _NPC_4_TOTAL_CHARS = 436;

	char _NPC_SUS_SELLER_TEXT[211] =
		"Hey...\0"
		"kid...\0"
		"you could have that upgrade\nbelow me for 1 spare change...\0"
		"It's quite...    \nNecessary?..\0"
		"Though,    \nyou won't approve of how\ni'm going to use the coin...\0"
		"(The upgrade can be used with\n'SHIFT')..";
	int _NPC_SUS_SELLER_TEXT_TOTAL_CHARS = 211;

	char _NPC_SUS_SELLER_TEXT2[48] =
		"yo...\0thanks for the coin...\0hehe...           ";
	int _NPC_SUS_SELLER_TEXT2_TOTAL_CHARS = 48;

	char _NPC_GUN_GIRL_TEXT[217] =
		"Hi, this is the\nGun upgrade store!\0"
		"Here you will find\nthe things that make\nor break your gunning!\0"
		"So, pick wisely,\nand you shall be happy with\nyour choice!\0"
		"(I do not take responsibility\nfor any misuse of these wares)";
	int _NPC_GUN_GIRL_TEXT_TOTAL_CHARS = 217;

	char _NPC_GUN_GIRL_TEXT2[215] =
		"Hi, this is the\nGun upgrade store!\0"
		"Hm,    \nit seems you do not have\na gun yet...\0"
		"How unfortunate.\0"
		"Still, you can always enjoy\nthe beauty of these upgrades...\0"
		"(As long as you're not one\nof those ANTI-gunning people)";
	int _NPC_GUN_GIRL_TEXT2_TOTAL_CHARS = 215;

	char _NPC_TREE_DUDE_TEXT[352] =
		"Oh, what a nice visit!\0"
		"Such trees like myself\nrarely see anyone\naround here.\0"
		"The only thing I see\nall day is that beautiful\nchest north of my leaves...\0"
		"It sure would be a shame\nif someone were to confiscate\nit...\0"
		"Luckily all my past visitors\nhave been nice,\0"
		"and kept their naugthy hands\naway from it...\0"
		"Hopefully, the same thing will\nbe said of you...";
	int _NPC_TREE_DUDE_TOTAL_CHARS = 352;

	char _NPC_BANANA_MAN_TEXT[287] =
		"Hi,      \nI'm Barry, the Banana man.\0"
		"Yeah, you've probably\nheard of me before right?\0"
		"I'm quite famous around here.\0"
		"(But more like INfamous)\nhehe...\0"
		"Well,      \nYou've come to the\nperfect place for shopping!\0"
		"Treat yourself to these\nbeautiful items!\0"
		"(More like 'BANANAeutiful'...)\nhehe...";
	int _NPC_BANANA_MAN_TOTAL_CHARS = 287;

	char _NPC_MOSQUITO_TEXT[118] =
		"Bzz   \0"
		"Bzzzz     \0"
		"You know you can\nchange the window size with\nO and P right?\0"
		"O and P, don't forget it         \nBzzzz";
	int _NPC_MOSQUITO_TOTAL_CHARS = 118;

private:
	inline void do_logic(Game& g, std::vector<MovingRect*>& vec);
public:

	EntityHandler();
	void logic(Game& g);
	void draw(Game& g);
	void place_enemy(Game& g, int x, int y);
	void place_npc(Game& g, NPC_TYPE::_ type, float x, float y);
};
