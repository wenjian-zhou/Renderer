cmake_minimum_required(VERSION 3.0)

project(Renderer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/core/color.h
    src/core/global.h
    src/core/hittable.h
    src/core/hittable_list.h
    src/core/main.cpp 
    src/core/ray.h
    src/core/sphere.h 
    src/core/vector.h 
    src/core/camera.h
    src/core/material.h  
    src/core/bvh.h
    src/core/texture.h
    src/core/global_stb_image.h 
    src/core/aarect.h 
    src/core/aabb.h
    src/core/box.h 
    src/core/constant_medium.h
    src/core/perlin.h
    src/core/moving_sphere.h
    src/core/triangle.h
    src/core/mesh_triangle.h
    src/external/tiny_obj_loader.h
    src/core/onb.h 
    src/core/pdf.h )

add_executable(Renderer ${SOURCES})