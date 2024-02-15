#pragma once

// builtin
#include <cassert>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

// local
#include "idx_vec2.hpp"



struct Tile {
    bool walkable;
};

class Map {
private:

    std::vector<std::vector<Tile>> tiles;
    IdxVec2 size;

private:

    Map(std::string const& map) : tiles{}, size{-1, -1} {

        static auto trim_string = [](std::string const& str) {
            std::string result;
            for (auto c: str)
                if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
                    result.push_back(c);

            return result;
        };

        static auto get_line_list = [](std::string const& map) {
            std::vector<std::string> lines;
            std::string line;
            std::istringstream stream(map);
            while (std::getline(stream, line)) {
                lines.push_back(trim_string(line));
            }
            return lines;
        };

        auto lines = get_line_list(map);

        // check that all lines have the same width
        auto const map_width = lines[0].size();
        for (auto& line: lines)
            assert(line.size() == map_width);

        // the map width should be at least 1
        assert(map_width > 0);

        this->size = {(int32_t)map_width, (int32_t)lines.size()};
        for (size_t x = 0; x < map_width; x++) {

            this->tiles.emplace_back();
            for (size_t y = 0; y < lines.size(); y++) {


                auto c = lines[y][x];
                auto tile = Tile{};
                if (c == '_')
                    tile = {true};
                else if (c == '#')
                    tile = {false};
                else
                    // invalid map character
                    assert(false);

                this->tiles[x].push_back(tile);
            }
        }
    }

public:

    [[nodiscard]]
    static Map from_file(std::filesystem::path const& path) {

        assert(std::filesystem::exists(path));
        std::ifstream file(path);
        std::string map;
        std::getline(file, map, '\0');
        return {map};
    };

    [[nodiscard]]
    IdxVec2 get_size() const {
        return this->size;
    }

    [[nodiscard]]
    bool is_walkable(IdxVec2 const& idx) const {
        return this->tiles[idx.x][idx.y].walkable;
    }

    [[nodiscard]]
    bool has_tile(IdxVec2 const& idx) const {
        return idx.x >= 0 && idx.x < this->size.x && idx.y >= 0 && idx.y < this->size.y;
    }

};
