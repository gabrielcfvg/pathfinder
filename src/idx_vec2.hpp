#pragma once

// builtin
#include <cstdint>



struct IdxVec2 {

public:

    int32_t x;
    int32_t y;

public:

        constexpr IdxVec2(int32_t x, int32_t y) : x(x), y(y) {}
        constexpr IdxVec2() : x(0), y(0) {}

        IdxVec2(IdxVec2 const& other) = default;
        IdxVec2(IdxVec2&& other) = default;
        IdxVec2& operator=(IdxVec2 const& other) = default;
        IdxVec2& operator=(IdxVec2&& other) = default;


        // vector arithmetic

        IdxVec2 operator+(IdxVec2 const& other) const {
                return {x + other.x, y + other.y};
        }

        IdxVec2 operator-(IdxVec2 const& other) const {
                return {x - other.x, y - other.y};
        }

        IdxVec2& operator+=(IdxVec2 const& other) {
                x += other.x;
                y += other.y;
                return *this;
        }

        IdxVec2& operator-=(IdxVec2 const& other) {
                x -= other.x;
                y -= other.y;
                return *this;
        }


        // comparison

        bool operator==(IdxVec2 const& other) const {
                return (x == other.x) && (y == other.y);
        }

        bool operator!=(IdxVec2 const& other) const {
                return (x != other.x) || (y != other.y);
        }
};
