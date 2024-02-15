// header
#include "idx_vec2.hpp"
#include "pathfinder.hpp"

// builtin
#include <optional>
#include <queue>
#include <cassert>




int32_t manhattan_distance(IdxVec2 lhs, IdxVec2 rhs) {
    return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
}

std::optional<Path> a_star(IdxVec2 orig, IdxVec2 dest, std::function<bool(IdxVec2)> is_tile_walkable, std::function<bool(IdxVec2)> is_tile_inside_limits) {

    struct Node {

        std::optional<IdxVec2> parent;
        bool visited;
        int32_t g;
        int32_t const h;

        Node(int32_t _g, std::optional<IdxVec2> _parent, IdxVec2 position, IdxVec2 dest)
        : parent(_parent), visited{false}, g{_g}, h(manhattan_distance(position, dest)) {}

        int32_t f() const {
            return g + h;
        }
    };

    std::unordered_map<IdxVec2, Node> nodes;

    auto const rebuild_path = [&]() -> Path {

        auto path = Path{};
        auto current = dest;
        while (current != orig) {
            path.push_back(current);
            current = nodes[current].parent.value();
        }

        std::reverse(path.begin(), path.end());
        return path;
    };

    auto const is_tile_walkable_and_inside_limits = [&](IdxVec2 tile) {
        return is_tile_walkable(tile) && is_tile_inside_limits(tile);
    };

    auto already_has_been_visited = [&](IdxVec2 tile) {
        auto it = nodes.find(tile);
        if (it == nodes.end())
            return false;
        return it->second.visited;
    };

    // the priority queue is a max heap, so we need to define a custom comparison function
    auto const node_cmp = [&](IdxVec2 lhs, IdxVec2 rhs) {
        return nodes[lhs].f() > nodes[rhs].f();
    };

    std::priority_queue<IdxVec2, std::vector<IdxVec2>, decltype(node_cmp)> pq{node_cmp};

    auto const push_node = [&](IdxVec2 position, Node node) {
        
        assert(nodes.find(position) == nodes.end() && "node already exists");
        nodes.insert({position, node});
        pq.push(position);
    };

    std::optional<Path> result = std::nullopt;

    // setup initial state
    nodes.insert({orig, Node{0, std::nullopt, orig, dest}});
    pq.push(orig);

    while (!result.has_value() && !pq.empty()) {
        
        // get the tile with the lowest f value and remove it from the queue
        auto current_tile = pq.top();
        pq.pop();

        // if the current tile is the destination, we found the path
        if (current_tile == dest) {
            result = rebuild_path();
            break;
        }

        // mark the current tile as visited
        nodes[current_tile].visited = true;

        // generate successors
        for (auto const neighbour: get_tile_neighbours(current_tile, is_tile_walkable_and_inside_limits)) {
            
            // ignore already visited tiles
            if (already_has_been_visited(neighbour.tile))
                continue;

            auto const new_g = nodes[current_tile].g + neighbour.cost;
            auto it = nodes.find(neighbour.tile);
            if (it == nodes.end()) { // if the neighbour is not in the open list, insert it
                nodes.insert({neighbour.tile, Node{new_g, current_tile, neighbour.tile, dest}});
                pq.push(neighbour.tile);
            }
            else if (new_g < it->second.g) { // if the neighbour is in the open list and the new g is lower, update it
                it->second.g = new_g;
                it->second.parent = current_tile;
            }
        }
    }

    return result;
}