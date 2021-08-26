cmake_minimum_required(VERSION 3.0)

project(render)

set(CMAKE_CXX_STANDARD 11)

add_executable(render
    main.cpp
    Vector.hpp
    Color.hpp
    Ray.hpp
    Hittable.hpp
    Sphere.hpp
    HittableList.hpp
    global.hpp
    Camera.hpp
    Material.hpp
    AABB.hpp
    BVH.hpp
    ConstantMedium.hpp
    Texture.hpp
)