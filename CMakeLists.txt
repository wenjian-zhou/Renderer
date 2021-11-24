cmake_minimum_required(VERSION 3.0)

project(Renderer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
    ./src/accelerators/aabb.h 
    ./src/accelerators/bvh.h 
    ./src/core/bsdf.h 
    ./src/core/bsdf.cpp 
    ./src/core/camera.h 
    ./src/core/global_stb_image.h 
    ./src/core/global.h 
    ./src/core/hittable_list.h 
    ./src/core/integrator.h 
    ./src/core/integrator.cpp 
    ./src/core/light.h 
    ./src/core/light.cpp 
    ./src/core/material.h 
    ./src/core/medium.h 
    ./src/core/medium.cpp 
    ./src/core/microfacet.h 
    ./src/core/microfacet.cpp 
    ./src/core/object.h 
    ./src/core/ray.h 
    ./src/core/record.h 
    ./src/core/renderer.h 
    ./src/core/renderer.cpp  
    ./src/core/sampler.h 
    ./src/core/scene.h 
    ./src/core/scene.cpp 
    ./src/core/spectrum.h 
    ./src/core/spectrum.cpp 
    ./src/core/transform.h 
    ./src/core/transform.cpp 
    ./src/core/vector.h 
    ./src/core/vector.cpp
    ./src/external/stb_image.h 
    ./src/external/tiny_obj_loader.h 
    ./src/integrators/path.h 
    ./src/integrators/path.cpp 
    ./src/lights/point.h 
    ./src/lights/point.cpp 
    ./src/lights/diffuse.h 
    ./src/lights/diffuse.cpp
    ./src/materials/matte.h 
    ./src/materials/matte.cpp 
    ./src/materials/glass.h 
    ./src/materials/glass.cpp
    ./src/shapes/triangle.h 
    ./src/shapes/aarect.h 
    ./src/main/main.cpp
)

add_executable(Renderer ${SOURCES})