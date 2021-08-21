cmake_minimum_required(VERSION 3.0)

project(render)

set(CMAKE_CXX_STANDARD 11)

add_executable(render
    main.cpp
    Vector.hpp
    Color.hpp
)