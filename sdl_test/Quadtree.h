#pragma once

#include <cinttypes>
#include "General.h"
#include <SDL.h>

// Type which to store pointers to. PER NODE, pointers til node splits, MAX_DEPTH
template<typename T, int PER_NODE, int MAX_DEPTH>
class Quadtree
{
private:
    std::vector<std::vector<T*>> _data;
    // positive for leaf node. Otherwise has 4 children, data[(negative value)*(-1)-1] equals where first childs data is
    std::vector<int32_t> _nodes;
    int32_t _head;
    int _x;
    int _y;
    int _w;
    int _h;

protected:

    int32_t node_add(int32_t index, T* elem, int x, int y, int w, int h, int depth);

    template <typename G>
    void node_do_intersection(int32_t index, G& g);

    template <typename G>
    void node_draw(int32_t index, G& g, int x, int y, int w, int h, int depth);

    void add_to_right_nodes(T* e, size_t node_index, int depth, int x, int y, int x2, int y2, int half_w, int half_h);

    int32_t add_nodes_and_reorganize(size_t index, T* elem, int x, int y, int w, int h, int depth);

public:
    Quadtree(int x, int y, int w, int h); 
    void add_to_head(T* elem);
    template <typename G>
    void head_do_intersection(G& g);
    template <typename G>
    void head_draw(G& g);
    void clear();
};

template <typename T, int PER_NODE, int MAX_DEPTH>
int32_t Quadtree<T, PER_NODE, MAX_DEPTH>::node_add(int32_t index, T* elem, int x, int y, int w, int h, int depth)
{
    int32_t new_index = index;
    if (index < 0)
    {
        // add to child nodes self
        int half_w = w / 2;
        int half_h = h / 2;
        this->add_to_right_nodes(elem, (size_t)(-index - 1), depth, x, y, x + half_w, y + half_h, half_w, half_h);
    }
    else if (_data[index].size() >= PER_NODE && depth < MAX_DEPTH)
    {
        // split, will add nodes and delete old data, while giving new nodes the data

        new_index = this->add_nodes_and_reorganize((size_t)index, elem, x, y, w, h, depth);

        // index will now be negative, and points to first node(of 4)
    }
    else
    {
        _data[index].push_back(elem);
    }
    return new_index;
}

template <typename T, int PER_NODE, int MAX_DEPTH>
template <typename G>
void Quadtree<T, PER_NODE, MAX_DEPTH>::node_do_intersection(int32_t index, G& g)
{
    if (index >= 0) // node has data
    {
        std::vector<T*>& elems = _data[index];
        
        for (size_t i = 0; i + 1 < elems.size(); ++i)
        {
            T* e1 = elems[i];
            for (size_t j = i + 1; j < elems.size(); ++j)
            {
                T* e2 = elems[j];
                if (General::rect_intersection<T>(*e1, *e2))
                {
                    // collision
                    float dx = e1->get_mid_x() - e2->get_mid_x();
                    float dy = e1->get_mid_y() - e2->get_mid_y();
                    float dist = sqrtf(dx * dx + dy * dy);

                    float nx = dx / dist;
                    float ny = dy / dist;

                    e1->intersection(g, nx, ny, e2);
                    e2->intersection(g, -nx, -ny, e1);
                    //break;
                }
            }
        }
    }
    else
    {
        // child nodes do intersection
        size_t real_index = ((size_t)(-index)) - 1;

        this->node_do_intersection(_nodes[real_index + 0], g);
        this->node_do_intersection(_nodes[real_index + 1], g);
        this->node_do_intersection(_nodes[real_index + 2], g);
        this->node_do_intersection(_nodes[real_index + 3], g);
    }
}

template <typename T, int PER_NODE, int MAX_DEPTH>
template <typename G>
void Quadtree<T, PER_NODE, MAX_DEPTH>::node_draw(int32_t index, G& g, int x, int y, int w, int h, int depth)
{
    if (index >= 0) // node has data
    {
        SDL_SetRenderDrawColor(g._renderer, 255, 0, 0, 255);

        SDL_Rect rect = { g._cam.convert_x(x), g._cam.convert_y(y), w, h };
        SDL_RenderDrawRect(g._renderer, &rect);
    }
    else
    {
        // child nodes do intersection
        size_t real_index = ((size_t)(-index)) - 1;

        int half_w = w / 2;
        int half_h = h / 2;
        int x2 = x + half_w;
        int y2 = y + half_h;

        this->node_draw(_nodes[real_index + 0], g, x, y, half_w, half_h, depth + 1);
        this->node_draw(_nodes[real_index + 1], g, x2, y, half_w, half_h, depth + 1);
        this->node_draw(_nodes[real_index + 2], g, x, y2, half_w, half_h, depth + 1);
        this->node_draw(_nodes[real_index + 3], g, x2, y2, half_w, half_h, depth + 1);
    }
}



template <typename T, int PER_NODE, int MAX_DEPTH>
void Quadtree<T, PER_NODE, MAX_DEPTH>::add_to_right_nodes(T* e, size_t node_index, int depth, int x, int y, int x2, int y2, int half_w, int half_h)
{
    int e_x = (int)e->get_x();
    int e_y = (int)e->get_y();
    int e_x2 = e_x + (int)e->get_w();
    int e_y2 = e_y + (int)e->get_h();

    if (e_x < x2) // on left side
    {
        if (e_y < y2) // on top left
        {
            _nodes[node_index + 0] = this->node_add(_nodes[node_index + 0], e, x, y, half_w, half_h, depth + 1);
        }
        if (e_y2 > y2) // bottom left
        {
            _nodes[node_index + 2] = this->node_add(_nodes[node_index + 2], e, x, y2, half_w, half_h, depth + 1);
        }
    }
    if (e_x2 > x2) // on right side
    {
        if (e_y < y2) // on top right
        {
            _nodes[node_index + 1] = this->node_add(_nodes[node_index + 1], e, x2, y, half_w, half_h, depth + 1);
        }
        if (e_y2 > y2) // bottom right
        {
            _nodes[node_index + 3] = this->node_add(_nodes[node_index + 3], e, x2, y2, half_w, half_h, depth + 1);
        }
    }
}

template <typename T, int PER_NODE, int MAX_DEPTH>
int32_t Quadtree<T, PER_NODE, MAX_DEPTH>::add_nodes_and_reorganize(size_t index, T* elem, int x, int y, int w, int h, int depth)
{
    size_t node_index = _nodes.size();

    {
        size_t data_index = _data.size();
        //_data.reserve(data_index + 4); // possibly uses less memory? but probably slower, cuz less forward 2^n allocations
        _data.resize(data_index + 4);
        _data[data_index + 0].reserve(PER_NODE);
        _data[data_index + 1].reserve(PER_NODE);
        _data[data_index + 2].reserve(PER_NODE);
        _data[data_index + 3].reserve(PER_NODE);

        int32_t data_index_32bit = (int32_t)data_index;

        //_nodes.reserve(node_index + 4); // could possibly make it slower?

        _nodes.push_back(data_index_32bit + 0);
        _nodes.push_back(data_index_32bit + 1);
        _nodes.push_back(data_index_32bit + 2);
        _nodes.push_back(data_index_32bit + 3);
    }


    int half_w = w / 2;
    int half_h = h / 2;
    int x2 = x + half_w;
    int y2 = y + half_h;

    for (T* e : _data[index])
    {
        this->add_to_right_nodes(e, node_index, depth, x, y, x2, y2, half_w, half_h);
    }

    this->add_to_right_nodes(elem, node_index, depth, x, y, x2, y2, half_w, half_h);

    
    // delete data (possibly)
    //_data[index].clear(); // possibly faster?

    //_data[index].shrink_to_fit(); // makes it 15% slower
    
    // will delete the data, and move all "pointers" to data to right point. makes it 26% slower
    /*_data.erase(_data.begin() + index);
    int32_t signed_index = (int32_t)index;
    for (size_t i = 0; i < _nodes.size(); ++i) { // doesn't make much memory difference unfortunately
        if (_nodes[i] > signed_index) {
            --_nodes[i];
        }
    }*/

    return ((-1) * ((int32_t)node_index)) - 1;
}
template <typename T, int PER_NODE, int MAX_DEPTH>
Quadtree<T, PER_NODE, MAX_DEPTH>::Quadtree(int x, int y, int w, int h) : _x(x), _y(y), _w(w), _h(h), _head(0) {}

template <typename T, int PER_NODE, int MAX_DEPTH>
void Quadtree<T, PER_NODE, MAX_DEPTH>::add_to_head(T* elem)
{
    _head = this->node_add(_head, elem, _x, _y, _w, _h, 0);
}

template <typename T, int PER_NODE, int MAX_DEPTH>
template <typename G>
void Quadtree<T, PER_NODE, MAX_DEPTH>::head_do_intersection(G& g)
{
    this->node_do_intersection(_head, g);
}

template <typename T, int PER_NODE, int MAX_DEPTH>
template <typename G>
void Quadtree<T, PER_NODE, MAX_DEPTH>::head_draw(G& g)
{
    this->node_draw(_head, g, _x, _y, _w, _h, 0);
}

// clears all data except where it exists, also creates first node
template <typename T, int PER_NODE, int MAX_DEPTH>
void Quadtree<T, PER_NODE, MAX_DEPTH>::clear()
{
    _nodes.clear();
    _data.clear();

    size_t index = _data.size();
    _data.emplace_back();
    //_data[index].reserve(PER_NODE); // could make it slower?

    _head = (int32_t)index;
}
