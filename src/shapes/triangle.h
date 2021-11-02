#ifndef RENDERER_TRIANGLE_H
#define RENDERER_TRIANGLE_H

#define TINYOBJLOADER_IMPLEMENTATION

#include "object.h"
#include "vector.h"
#include "../external/tiny_obj_loader.h"

class Triangle : public Object
{
public:
    Triangle() {}
    Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2, shared_ptr<Material> m) : v0(_v0), v1(_v1), v2(_v2), mat_ptr(m)
    {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = Normalize(Cross(e1, e2));
        area = Cross(e1, e2).Length() * 0.5f;
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;
    bool Intersect(const Ray &ray, HitRecord *isect) const;

public:
    Vector3f v0, v1, v2; // vertices A, B, C, counter-clockwise order
    Vector3f e1, e2; // 2 edges v1-v0, v2-v0
    Vector3f t0, t1, t2; // texture coords
    Vector3f normal;
    double area;
    std::shared_ptr<Material> mat_ptr;
};

class TriangleMesh : public Triangle
{
public:
    TriangleMesh() {}
    TriangleMesh(std::string inputfile, std::shared_ptr<Material> mat_ptr)
    {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = inputfile;

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
                std::vector<Vector3f> vertices;
                for (size_t v = 0; v < fv; v ++)
                {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                    vertices.push_back(Vector3f(vx, vy, vz));
                }
                Triangle face = Triangle(vertices[0], vertices[1], vertices[2], mat_ptr);
                Triangles.push_back(face);
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }
    }
public:
    std::vector<Triangle> Triangles;
};


#endif