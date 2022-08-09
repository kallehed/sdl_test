#pragma once

enum class PLAYER_WEAPON : char {
	FIRE_MAGIC = 0,
	GUN,

	TOTAL
};

enum class MOVING_RECT_TYPES : char {
	PLAYER = 0,
	ENEMY,
	SHOT,
	COIN,
	PICKUPABLE,
	BOMB,
	EXPLOSION,
	FIRE_MAGIC,
	NPC,
	PORTAL,

	TOTAL
};

// Represents function + shape of tile
namespace TILE {
	enum TILE : char {
		VOID = 0, // higher elements are not walkthrough, and are drawn
		BLOCK,
		DESTRUCTABLE,

		TRI_NE, // triangle, hypothenuse towards north east
		TRI_SE, // south east,
		TRI_NW, // north west,
		TRI_SW, // ...

		TOTAL
	};
}

// Textures. WARNING: ONLY ADD TEXTURES TO END!!!! OTHERWISE TEXTURES WILL BE SWAPPED WHEN LOADING SAVE
namespace TEX {
	enum TEX : char {
		VOID = -1,
		FireMagic = 0,
		BombExplosion,
		Bomb,
		BombRed,
		Bullet,
		MagicOrb,
		BlueSlime,
		Wizard,
		GreenSlime1,
		GreenSlime2,
		RedBear,
		RedHuman,
		RedHumanWalk1,
		RedHumanWalk2,
		RedHumanBackwards,
		RedHumanBackwardsWalk1,
		RedHumanBackwardsWalk2,
		Coin,
		Container,
		DialogueBox,

		Bush, // Beginning of tile images is at TEX::Bush, except, NOT
		Bush2,
		TreeStump,
		SmallTree1,
		SmallTree2,
		SquareGreen,
		SquareStone,
		SquareSwamp,
		CobbleStone,
		CobbleWall,
		FlowerRed,
		FlowerBlue1,
		FlowerBlue2,

		Portal,
		Heart,
		
		TOTAL
	};
}

// Pickupables
enum class PICKUPABLE_TYPE {
	COIN,
	SHOT,
	BOMB,
	HEART,

	TOTAL
};

// Camera stuff
namespace EDIT_MODE {
	enum EDIT_MODE : char {
		TILE,
		TEX,
		ENTITY,

		TOTAL
	};
}

namespace ENTITIES {
	enum ENTITIES : char {
		ENEMY_BASIC,
		ENEMY_SHOOTER,
		NPC,
		PORTAL,

		TOTAL
	};
}

// Button
namespace BTN {
	enum BTN : char {
		NOTHING = 0,
		HELD_OVER,
		CLICKED_ON,

		TOTAL
	};
}

namespace CAM_BTN {
	enum CAM_BTN : char {
		SAVE,
		LOAD,
		VIEW,
		SET_POS,

		TOTAL
	};
}

namespace CAM_VIEW {
	enum CAM_VIEW : char {
		TEX,
		SHAPE,
		HALF,

		TOTAL
	};
}

namespace LEVEL {
	enum LEVEL : char {
		START,
		LEVEL1,
	};
}