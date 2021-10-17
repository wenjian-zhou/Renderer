#ifndef MESH_TRIANGLE_H
#define MESH_TRIANGLE_H

#define TINYOBJLOADER_IMPLEMENTATION

#include "triangle.h"
#include "../external/tiny_obj_loader.h"

class mesh_triangle : public triangle
{
public:
    mesh_triangle() {}
    mesh_triangle(std::string inputfile, std::shared_ptr<material> mat_ptr)
    {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "../models/bunny/";

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(inputfile, reader_config)) 
        {
            if (!reader.Error().empty()) 
            {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            exit(1);
        }

        if (!reader.Warning().empty())
        {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();
        auto &materials = reader.GetMaterials();

        // Loop over shapes
        for (size_t s = 0;  s < shapes.size(); s ++)
        {
            // Loop over faces (polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f ++)
            {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face
                std::vector<vec3> vertices;
                for (size_t v = 0; v < fv; v ++)
                {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                    vertices.push_back(vec3(vx, vy, vz));
                }
                triangle face = triangle(vertices[0], vertices[1], vertices[2], mat_ptr);
                triangles.push_back(face);
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }
    }
public:
    std::vector<triangle> triangles;
};

#endif