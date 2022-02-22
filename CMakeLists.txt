cmake_minimum_required(VERSION 3.0)



project(Renderer)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

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
    ./src/integrators/volpath.h 
    ./src/integrators/volpath.cpp
    ./src/lights/point.h 
    ./src/lights/point.cpp 
    ./src/lights/diffuse.h 
    ./src/lights/diffuse.cpp
    ./src/materials/matte.h 
    ./src/materials/matte.cpp 
    ./src/materials/glass.h 
    ./src/materials/glass.cpp
    ./src/materials/metal.h 
    ./src/materials/metal.cpp
    ./src/materials/plastic.h 
    ./src/materials/plastic.cpp
    ./src/shapes/triangle.h 
    ./src/shapes/aarect.h 
    ./src/shapes/sphere.h
    ./src/medium/homogeneous.h 
    ./src/medium/homogeneous.cpp 
    ./src/main/main.cpp
)

add_executable(Renderer ${SOURCES})