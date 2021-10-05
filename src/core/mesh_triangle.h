#ifndef MESH_TRIANGLE_H
#define MESH_TRIANGLE_H

#include "triangle.h"
#include "../external/tiny_obj_loader.h"

class mesh_triangle : public triangle
{
public:
    mesh_triangle() {}
public:
    std::vector<triangle> triangles;
    std::vector<double> normals;
    std::shared_ptr<material> mat_ptr;
};

#endif