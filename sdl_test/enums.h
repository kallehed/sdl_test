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
	PICKUPABLE_SHOT,
	BOMB,
	EXPLOSION,
	FIRE_MAGIC,

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
		BombExplosion = 1,
		Bomb = 2,
		Bullet = 3,
		MagicOrb = 4,
		BlueSlime = 5,
		Wizard = 6,
		GreenSlime1 = 7,
		GreenSlime2 = 8,
		RedHuman = 9,
		Coin = 10,
		Container = 11,

		Bush = 12, // Beginning of tile images is at TEX::Bush, except, NOT
		Bush2 = 13,
		TreeStump = 14,
		SmallTree1 = 15,
		SmallTree2 = 16,
		GreenSquare = 17,
		CobbleStone = 18,
		CobbleWall = 19,

		BombRed = 20,

		TOTAL = 21
	};
}

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