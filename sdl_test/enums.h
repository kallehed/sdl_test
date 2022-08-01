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

// Textures
namespace TEX {
	enum TEX : char {
		VOID = -1,
		FireMagic = 0,
		BombExplosion,
		Bullet,
		BlueSlime,
		GreenSlime1,
		GreenSlime2,
		RedHuman,
		Coin,
		Container,

		Bush, // Beginning of tile images is at TEX::Bush
		Bush2,
		TreeStump,
		SmallTree1,
		SmallTree2,
		GreenSquare,

		TOTAL
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
		BOTH,

		TOTAL
	};
}