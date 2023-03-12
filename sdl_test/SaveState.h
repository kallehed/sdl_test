#pragma once

#include <array>
#include <unordered_set>
#include <cstddef>

// Hash for std::array<int, 2>
struct HashArray2Int {
public:
	size_t operator()(std::array<int, 2> ar) const {
		int val = ar[0] * ar[1];
		return std::hash<int>()(val);
	}
};
// Custom comparator that compares the array objects
struct EqualArray2Int {
public:
	bool operator()(std::array<int, 2> ar1, std::array<int, 2> ar2) const {
		if (ar1 == ar2)
			return true;
		else
			return false;
	}
};

class SaveState
{
public:

	// onetime_index stuff
	// So the first number represents the level
	// The second number represents index in which something appears
	std::unordered_set<std::array<int, 2>, HashArray2Int, EqualArray2Int> _onetimes;

	// Which red bear should spawn
	int _red_bear_stage;

	// 0 when should talkative, 1 when have run away(therefore invisible)
	int _sus_seller_stage;

	SaveState();
	void reset();
};

