cmake_minimum_required(VERSION 3.27)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(pathfinder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pathfinder
        src/main.cpp
        src/pathfinder.hpp
        src/idx_vec2.hpp
        src/map.hpp
        src/a_star.cpp
        src/dijkstra.cpp
)
