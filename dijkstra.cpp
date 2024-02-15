// header
#include "pathfinder.hpp"

// builtin
#include <queue>
#include <vector>
#include <unordered_map>


template<>
struct std::less<std::pair<int32_t, IdxVec2>> {
    bool operator()(std::pair<int32_t, IdxVec2> const& lhs, std::pair<int32_t, IdxVec2> const& rhs) const {
        return lhs.first > rhs.first;
    }
};

std::optional<Path> djikstra(IdxVec2 orig, IdxVec2 dest, std::function<bool(IdxVec2)> is_tile_walkable, std::function<bool(IdxVec2)> is_tile_inside_limits) {

    std::priority_queue<std::pair<int32_t, IdxVec2>, std::vector<std::pair<int32_t, IdxVec2>>, std::less<std::pair<int32_t, IdxVec2>>> pq;
    std::unordered_map<IdxVec2, int32_t> __dist;

    auto const set_dist = [&](IdxVec2 tile, int32_t dist) {
        __dist[tile] = dist;
    };

    auto const get_dist = [&](IdxVec2 tile) {
        auto it = __dist.find(tile);
        if (it == __dist.end())
            return INT32_MAX;
        return it->second;
    };

    auto const rebuild_path = [&]() -> Path {

        // check if the destination is reachable
        assert(get_dist(dest) != INT32_MAX);

        auto path = Path{};
        auto current = dest;
        while (current != orig) {
            path.push_back(current);
            auto const current_dist = get_dist(current);
            for (auto const neighbour: get_tile_neighbours(current, is_tile_inside_limits)) {
                if (get_dist(neighbour.tile) == current_dist - neighbour.cost) {
                    current = neighbour.tile;
                    break;
                }
            }
            assert(current != path.back() && "at least one neighbour should have the expected distance");
        }

        std::reverse(path.begin(), path.end());
        return path;
    };

    // init origin tile
    set_dist(orig, 0);
    pq.push({0, orig});

    std::optional<Path> result = std::nullopt;

    while (!result.has_value() && !pq.empty()) {
        auto [current_dist, current_tile] = pq.top();
        pq.pop();

        if (current_tile == dest) {
            result = rebuild_path();
            break;
        }

        for (auto const neighbour: get_tile_neighbours(current_tile, is_tile_inside_limits)) {
            if (!is_tile_walkable(neighbour.tile))
                continue;

            auto const new_dist = current_dist + 1;
            if (new_dist < get_dist(neighbour.tile)) {
                set_dist(neighbour.tile, new_dist);
                pq.push({new_dist, neighbour.tile});
            }
        }
    }

    return result;
}