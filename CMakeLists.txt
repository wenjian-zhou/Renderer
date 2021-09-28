cmake_minimum_required(VERSION 3.0)

project(Renderer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
    src/color.h
    src/global.h
    src/hittable.h
    src/hittable_list.h
    src/main.cpp 
    src/ray.h
    src/sphere.h 
    src/vec3.h 
    src/camera.h
    src/material.h  
    src/bvh.h
    src/texture.h
<<<<<<< HEAD
    src/global_stb_image.h src/aarect.h src/box.h src/constant_medium.h)
=======
    src/global_stb_image.h src/aarect.h src/box.h src/constant_medium.h src/core/vector.h)
>>>>>>> parent of 267aa56 (update integrator and perlin)

add_executable(Renderer ${SOURCES})