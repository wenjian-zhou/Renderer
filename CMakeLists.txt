cmake_minimum_required(VERSION 3.0)

project(Renderer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
<<<<<<< HEAD
    src/core/main.cpp 
    src/core/color.h
    src/core/global.h
    src/core/hittable.h
    src/core/hittable_list.h
    src/core/ray.h
    src/core/sphere.h 
    src/core/vec3.h 
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
    src/core/integrator.h 
    src/core/renderer.h 
    src/core/renderer.cpp 
    )
=======
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
    src/global_stb_image.h src/aarect.h src/box.h src/constant_medium.h src/core/vector.h)
>>>>>>> parent of 267aa56 (update integrator and perlin)

add_executable(Renderer ${SOURCES})