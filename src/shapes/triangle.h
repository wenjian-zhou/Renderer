#ifndef RENDERER_TRIANGLE_H
#define RENDERER_TRIANGLE_H

#include "../core/object.h"

class Triangle : public Object
{
public:
    Triangle(Vector3f _v0, Vector3f _v1, Vector3f _v2, shared_ptr<Material> m, std::shared_ptr<MediumRecord> mediumRecord = nullptr)
         : v0(_v0), v1(_v1), v2(_v2), mat_ptr(m), Object(mediumRecord) {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = Normalize(Cross(e1, e2));
        area = Cross(e1, e2).Length() * 0.5f;
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB &output_box) const override;
    virtual bool Intersect(const Ray &ray, HitRecord &isect) const override;

public:
    Vector3f v0, v1, v2; // vertices A, B, C, counter-clockwise order
    Vector3f e1, e2; // 2 edges v1-v0, v2-v0
    Vector3f t0, t1, t2; // texture coords
    Vector3f normal;
    double area;
    std::shared_ptr<Material> mat_ptr;
};

bool Triangle::Intersect(const Ray &ray, HitRecord &isect) const {
    Vector3f edge1 = v1 - v0;
    Vector3f edge2 = v2 - v0;
    Vector3f pvec = Cross(ray.d, edge2);
    float det = AbsDot(edge1, pvec);
    if (det == 0)
        return false;

    Vector3f tvec = ray.o - v0;
    isect.u = Dot(tvec, pvec);
    if (isect.u < 1 - ShadowEpsilon || isect.u > det)
        return false;

    Vector3f qvec = Cross(tvec, edge1);
    isect.v = Dot(ray.d, qvec);
    if (isect.v < 1 - ShadowEpsilon || isect.u + isect.v > det)
        return false;

    float invDet = 1 / det;

    if (ray.tMax <= Dot(edge2, qvec) * invDet || Dot(edge2, qvec) * invDet < 0.0001f) return false;

    ray.tMax = Dot(edge2, qvec) * invDet;
    isect.t = ray.tMax;
    isect.p = ray(isect.t);
    isect.u *= invDet;
    isect.v *= invDet;
    isect.normal = normal;
    isect.mat_ptr = mat_ptr;
    isect.wo = -ray.d;
    return true;
}

bool Triangle::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    Vector3f edge1 = v1 - v0;
    Vector3f edge2 = v2 - v0;
    Vector3f pvec = Cross(r.d, edge2);
    float det = Dot(edge1, pvec);
    if (det == 0 || det < 0)
        return false;

    Vector3f tvec = r.o - v0;
    rec.u = Dot(tvec, pvec);
    if (rec.u < 0 || rec.u > det)
        return false;

    Vector3f qvec = Cross(tvec, edge1);
    rec.v = Dot(r.d, qvec);
    if (rec.v < 0 || rec.u + rec.v > det)
        return false;

    float invDet = 1 / det;

    rec.t = Dot(edge2, qvec) * invDet;
    rec.p = r(rec.t);
    rec.u *= invDet;
    rec.v *= invDet;
    rec.normal = normal;
    rec.mat_ptr = mat_ptr;
    //std::cout << rec.normal << std::endl;
    return true;
}

bool Triangle::bounding_box(double time0, double time1, AABB &output_box) const
{
    Vector3f min = Vector3f(
        std::min(v0.x, std::min(v1.x, v2.x)),
        std::min(v0.y, std::min(v1.y, v2.y)),
        std::min(v0.z, std::min(v1.z, v2.z))
    );

    Vector3f max = Vector3f(
        std::max(v0.x, std::max(v1.x, v2.x)),
        std::max(v0.y, std::max(v1.y, v2.y)),
        std::max(v0.z, std::max(v1.z, v2.z))
    );

    output_box = AABB(Vector3f(min.x - 0.00001f, min.y - 0.00001f, min.z - 0.00001f), 
                      Vector3f(max.x + 0.00001f, max.y + 0.00001f, max.z + 0.00001f));

    return true;
}

#define TINYOBJLOADER_IMPLEMENTATION
#include "../external/tiny_obj_loader.h"

class TriangleMesh : public Object
{
public:
    TriangleMesh(const float &rotate_angle, const Vector3f &translate, const float &scale, std::string inputfile, std::string mtlsource, std::shared_ptr<Material> mat_ptr, std::shared_ptr<MediumRecord> mediumRecord = nullptr)
        : Object(mediumRecord) {
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = mtlsource;

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

        float angle = rotate_angle / 180.f * PI;
        Matrix4x4 rotateByY = Matrix4x4(
            cos(angle), 0, sin(angle), 0,
            0, 1, 0, 0,
            -sin(angle), 0, cos(angle), 0,
            0, 0, 0, 1
        );
        Transform rotate = Transform(rotateByY);

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

                Triangle face = Triangle((vertices[0] * scale) + translate, (vertices[1] * scale) + translate, (vertices[2] * scale) + translate, mat_ptr, mediumRecord);
                Triangles.push_back(face);
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }
    }

    virtual bool hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const {}
    virtual bool bounding_box(double time0, double time1, AABB &output_box) const {}
    virtual bool Intersect(const Ray &ray, HitRecord &isect) const override {}
public:
    std::vector<Triangle> Triangles;
};


#endif