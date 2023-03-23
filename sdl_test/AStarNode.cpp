#include "AStarNode.h"
#include "Game.h"
#include <array>

void AStarNode::set_variables(int G, int H)
{
	_G = G;
	_H = H;
	_F = G + H;
}

// true if it found path, false if no path
std::vector<std::array<int, 2>> AStarNode::generate_walk_path(Game& g, float x2, float y2, float x1, float y1)
{
	// clear it, no reason not to
	std::vector<std::array<int, 2>> path_vec;

	// global coordinates for start and end positions
	int start_i = g._cam.convert_y_to_i(y1);
	int start_j = g._cam.convert_x_to_j(x1);
	int end_i = g._cam.convert_y_to_i(y2);
	int end_j = g._cam.convert_x_to_j(x2);

	const int k = 11; // side length of 2D array

	AStarNode nodes[k][k]; // 2D array of nodes

	// where the 2d grid begins in global coordinates
	int global_left_j = start_j - (k + start_j - end_j) / 2;
	int global_top_i = start_i - (k + start_i - end_i) / 2;

	// i and j in nodes 2D array
	int local_start_i = start_i - global_top_i;
	int local_start_j = start_j - global_left_j;
	int local_end_i = end_i - global_top_i;
	int local_end_j = end_j - global_left_j;

	if (local_start_i < 0 || local_start_i >= k || local_start_j < 0 || local_start_j >= k ||
		local_end_i < 0 || local_end_i >= k || local_end_j < 0 || local_end_j >= k) {
		// too far apart from start to end, so it doesn't fit in grid
		return path_vec;
	}

	nodes[local_start_i][local_start_j].set_variables(0, 0);
	nodes[local_start_i][local_start_j]._i = local_start_i;
	nodes[local_start_i][local_start_j]._j = local_start_j;

	// open nodes, start with start node
	std::vector<AStarNode*> open_nodes = { &nodes[local_start_i][local_start_j] };

	const std::pair<const int, const int> walk_ways[] = { {1,0},{0,1},{-1,0},{0,-1} };

	while (true) {
		// get node with lowest F value from open_nodes
		AStarNode* best_node = nullptr;
		{
			int lowest_F = INT_MAX;
			int lowest_H = INT_MAX;
			for (auto node : open_nodes)
			{
				if (node->_F < lowest_F || (node->_F == lowest_F && node->_H < lowest_H))
				{
					best_node = node;
					lowest_F = node->_F;
					lowest_H = node->_H;
				}
			}
		}
		if (best_node == nullptr)
		{
			// no path to the end
			return path_vec;
		}
		// close the node, remove it from
		best_node->_closed = true;
		//closed_nodes.push_back(best_node);
		open_nodes.erase(std::remove(open_nodes.begin(), open_nodes.end(), best_node), open_nodes.end());

		// go through neighbors
		for (auto& walk_way : walk_ways) {
			// i and j of neighbor node (local)
			int i = best_node->_i + walk_way.first;
			int j = best_node->_j + walk_way.second;
			if (i >= 0 && i < k && j >= 0 && j < k) { // inside 2D array
				if (!g._tile_handler.is_blocking_tile(global_top_i + i, global_left_j + j)) { // not a blocking tile

					if (i == local_end_i && j == local_end_j) {
						// reached end
						path_vec.reserve(best_node->_G + (size_t)2);
						path_vec.emplace_back(std::array{ global_top_i + i,  global_left_j + j }); // last place
						AStarNode* path_node = best_node;
						while (path_node != nullptr) {
							path_vec.emplace_back(std::array{ global_top_i + path_node->_i ,  global_left_j + path_node->_j });
							path_node = path_node->_parent;
						}
						return path_vec;
					}

					AStarNode* neighbor_node = &nodes[i][j];
					if (!neighbor_node->_closed) { // only check open nodes or untouched
						int new_G = best_node->_G + 1;
						if (neighbor_node->_parent == nullptr) {
							// untouched node, define without question
							neighbor_node->_i = i;
							neighbor_node->_j = j;
							neighbor_node->_parent = best_node;
							// define H
							int new_H = abs(local_end_i - i) + abs(local_end_j - j);
							neighbor_node->set_variables(new_G, new_H);
							open_nodes.push_back(neighbor_node);
						}
						else {
							// it's open, but already touched by other closed node
							// therefore check if it should be updated, if this path is better
							if (new_G < neighbor_node->_G) {
								neighbor_node->_G = new_G;
								neighbor_node->_parent = best_node;
							}
						}
					}
				}
			}
		}
	}
}