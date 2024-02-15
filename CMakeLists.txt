cmake_minimum_required(VERSION 3.27)
project(pathfinder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pathfinder main.cpp
        pathfinder.hpp
        idx_vec2.hpp
        map.hpp
        a_star.cpp
        dijkstra.cpp
)
