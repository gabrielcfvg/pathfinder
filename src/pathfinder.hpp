#pragma once

// builtin
#include <vector>
#include <optional>
#include <unordered_set>
#include <array>
#include <functional>

// local
#include "idx_vec2.hpp"



template<>
struct std::hash<IdxVec2> {
    std::size_t operator()(IdxVec2 const& vec) const {
        return std::hash<int32_t>()(vec.x * vec.y);
    }
};

using Path = std::vector<IdxVec2>;

inline bool is_path_valid(Path const& path) {

    for (size_t i = 0; i < path.size() - 1; i++) {
        auto const& current = path[i];
        auto const& next = path[i + 1];
        auto const diff = next - current;

        if (diff == IdxVec2{0, 0})
            return false;

        if (diff.x < -1 || diff.x > 1)
            return false;

        if (diff.y < -1 || diff.y > 1)
            return false;
    }

    std::unordered_set<IdxVec2> used_tiles;
    for (auto const& tile: path) {
        if (used_tiles.find(tile) != used_tiles.end())
            return false;
        used_tiles.insert(tile);
    }

    return true;
}

inline consteval std::array<IdxVec2, 8> potential_neighbours_offsets() {

    size_t idx = 0;
    std::array<IdxVec2, 8> offsets;

    for (int32_t y = -1; y <= 1; y++) {
        for (int32_t x = -1; x <= 1; x++) {
            if (x == 0 && y == 0)
                continue;
            offsets[idx++] = {x, y};
        }
    }

    return offsets;
}

inline int32_t const LINEAR_COST = 10;
inline int32_t const DIAGONAL_COST = 14;

struct Neighbour {
    IdxVec2 tile{};
    int32_t cost{};
};

inline std::vector<Neighbour> get_tile_neighbours(IdxVec2 tile, std::function<bool(IdxVec2)> is_tile_inside_limits) {

    static constinit auto const offsets = potential_neighbours_offsets();

    auto const get_offset_cost = [](IdxVec2 offset) {
        if (offset.x == 0 || offset.y == 0)
            return LINEAR_COST;
        return DIAGONAL_COST;
    };

    std::vector<Neighbour> neighbours;
    neighbours.reserve(8);

    for (size_t i = 0; i < 8; ++i) {
        auto potential_neighbour = tile + offsets[i];
        if (is_tile_inside_limits(potential_neighbour))
            neighbours.emplace_back(Neighbour{potential_neighbour, get_offset_cost(offsets[i])});
    }

    return neighbours;
}


std::optional<Path> a_star_manhattan(IdxVec2 orig, IdxVec2 dest, std::function<bool(IdxVec2)> is_tile_walkable, std::function<bool(IdxVec2)> is_tile_inside_limits);
std::optional<Path> a_star_euclidean(IdxVec2 orig, IdxVec2 dest, std::function<bool(IdxVec2)> is_tile_walkable, std::function<bool(IdxVec2)> is_tile_inside_limits);
std::optional<Path> djikstra(IdxVec2 orig, IdxVec2 dest, std::function<bool(IdxVec2)> is_tile_walkable, std::function<bool(IdxVec2)> is_tile_inside_limits);
