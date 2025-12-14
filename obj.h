#ifndef OBJ_H
#define OBJ_H

#include "raytracer.h"
#include "hittable.h"
#include "hittable_list.h"
#include "bvh.h"
#include "vec2.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

// Triangle primitive
class triangle : public hittable
{
public:
    triangle(const point3 &a, const point3 &b, const point3 &c,
             const vec3 &na, const vec3 &nb, const vec3 &nc,
             const vec2 &ta, const vec2 &tb, const vec2 &tc,
             shared_ptr<material> m)
    {
        // Verticies
        v0 = a;
        v1 = b;
        v2 = c;

        // Normals
        n0 = na;
        n1 = nb;
        n2 = nc;

        // Text Coords
        t0 = ta;
        t1 = tb;
        t2 = tc;

        // Material
        mat = m;

        // Bounding box for the triangle
        point3 minp(
            std::fmin(v0.x, std::fmin(v1.x, v2.x)),
            std::fmin(v0.y, std::fmin(v1.y, v2.y)),
            std::fmin(v0.z, std::fmin(v1.z, v2.z)));
        point3 maxp(
            std::fmax(v0.x, std::fmax(v1.x, v2.x)),
            std::fmax(v0.y, std::fmax(v1.y, v2.y)),
            std::fmax(v0.z, std::fmax(v1.z, v2.z)));

        const auto eps = 1e-4;
        bbox = aabb(minp - vec3(eps, eps, eps), maxp + vec3(eps, eps, eps));
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        // Moller–Trumbore intersection
        constexpr double eps = 1e-8;

        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        vec3 pvec = cross(r.direction, e2);
        double det = dot(e1, pvec);

        if (std::fabs(det) < eps)
            return false;

        double inv_det = 1.0 / det;

        vec3 tvec = r.origin - v0;
        double u = dot(tvec, pvec) * inv_det;
        if (u < 0.0 || u > 1.0)
            return false;

        vec3 qvec = cross(tvec, e1);
        double v = dot(r.direction, qvec) * inv_det;
        if (v < 0.0 || (u + v) > 1.0)
            return false;

        double t = dot(e2, qvec) * inv_det;
        if (!ray_t.contains(t))
            return false;

        // Fill hit record
        rec.t = t;
        rec.p = r.at(t);
        rec.mat = mat;

        // Barycentric normal interpolation
        vec3 interp_n = (1.0 - u - v) * n0 + u * n1 + v * n2;
        if (interp_n.length_squared() < eps)
        {
            interp_n = unit_vector(cross(e1, e2));
        }
        else
        {
            interp_n = unit_vector(interp_n);
        }

        rec.set_face_normal(r, interp_n);
        // Calculate uv
        double w = 1.0 - u - v;
        vec2 uv = w * t0 + u * t1 + v * t2;
        rec.u = uv.x;
        rec.v = uv.y;

        // If material is transparent here
        if (rec.mat && !rec.mat->accept_hit(rec.u, rec.v, rec.p))
            return false;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    point3 v0, v1, v2;        // verticies
    vec2 t0, t1, t2;          // uv of each vert
    vec3 n0, n1, n2;          // normal vector of each vert
    shared_ptr<material> mat; // material
    aabb bbox;                // bounding box
};

// Obj Mesh
class obj : public hittable
{
public:
    // Loads from "models/<filename>"
    explicit obj(const std::string &filename,
                 shared_ptr<material> mat,
                 const std::string &base_dir = "models/")
    {
        load_from_file(base_dir + filename, mat);
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        return accel ? accel->hit(r, ray_t, rec) : false;
    }

    aabb bounding_box() const override { return bbox; }

private:
    shared_ptr<hittable> accel; // BVH root
    aabb bbox;

private:
    static inline void ltrim(std::string &s)
    {
        size_t i = 0;
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
            ++i;
        s.erase(0, i);
    }

    // Parses obj file to get data
    static inline int parse_index(const std::string &tok, int which, bool &present)
    {
        present = false;

        int part = 0;
        std::string cur;
        for (size_t i = 0; i <= tok.size(); ++i)
        {
            char c = (i < tok.size()) ? tok[i] : '/';
            if (c == '/')
            {
                if (part == which)
                {
                    if (!cur.empty())
                    {
                        present = true;
                        return std::stoi(cur);
                    }
                    return 0;
                }
                cur.clear();
                part++;
            }
            else
            {
                cur.push_back(c);
            }
        }
        return 0;
    }

    void load_from_file(const std::string &path, shared_ptr<material> mat)
    {
        std::ifstream in(path);
        if (!in)
        {
            throw std::runtime_error("Failed to open OBJ: " + path);
        }

        std::vector<point3> positions;
        std::vector<vec3> normals;
        std::vector<vec3> texcoords; // (u,v,0) if you want later

        hittable_list tris;

        std::string line;
        while (std::getline(in, line))
        {
            ltrim(line);
            if (line.empty() || line[0] == '#')
                continue;

            std::istringstream ss(line);
            std::string tag;
            ss >> tag;

            if (tag == "v")
            {
                double x, y, z;
                ss >> x >> y >> z;
                positions.emplace_back(x, y, z);
            }
            else if (tag == "vn")
            {
                double x, y, z;
                ss >> x >> y >> z;
                normals.emplace_back(x, y, z);
            }
            else if (tag == "vt")
            {
                double u, v;
                ss >> u >> v;
                texcoords.emplace_back(u, v, 0.0);
            }
            else if (tag == "f")
            {
                // Read all face vertices, then triangulate fan
                std::vector<std::string> verts;
                std::string tok;
                while (ss >> tok)
                    verts.push_back(tok);
                if (verts.size() < 3)
                    continue;

                auto fetch_pos = [&](int idx) -> point3
                {
                    if (idx > 0)
                        return positions[idx - 1];
                    return positions[positions.size() + idx];
                };
                auto fetch_nrm = [&](int idx) -> vec3
                {
                    if (normals.empty())
                        return vec3(0, 0, 0);
                    if (idx > 0)
                        return normals[idx - 1];
                    return normals[normals.size() + idx];
                };

                auto fetch_uv = [&](int idx) -> vec2
                {
                    if (texcoords.empty())
                        return vec2(0, 0);
                    if (idx > 0)
                        return vec2(texcoords[idx - 1].x, texcoords[idx - 1].y);
                    auto t = texcoords[texcoords.size() + idx];
                    return vec2(t.x, t.y);
                };

                // Fan triangulation: (0,i,i+1)
                for (size_t i = 1; i + 1 < verts.size(); ++i)
                {
                    bool has_v0, has_v1, has_v2;
                    bool has_n0, has_n1, has_n2;
                    bool has_t0, has_t1, has_t2;

                    int v0i = parse_index(verts[0], 0, has_v0);
                    int v1i = parse_index(verts[i], 0, has_v1);
                    int v2i = parse_index(verts[i + 1], 0, has_v2);

                    int n0i = parse_index(verts[0], 2, has_n0);
                    int n1i = parse_index(verts[i], 2, has_n1);
                    int n2i = parse_index(verts[i + 1], 2, has_n2);

                    int t0i = parse_index(verts[0], 1, has_t0);
                    int t1i = parse_index(verts[i], 1, has_t1);
                    int t2i = parse_index(verts[i + 1], 1, has_t2);

                    if (!has_v0 || !has_v1 || !has_v2)
                        continue;

                    point3 a, b, c;
                    vec3 na, nb, nc;
                    vec2 ta, tb, tc;

                    a = fetch_pos(v0i);
                    b = fetch_pos(v1i);
                    c = fetch_pos(v2i);

                    if (has_n0)
                        na = fetch_nrm(n0i);
                    if (has_n1)
                        nb = fetch_nrm(n1i);
                    if (has_n2)
                        nc = fetch_nrm(n2i);
                    if (has_t0)
                        ta = fetch_uv(t0i);
                    if (has_t1)
                        tb = fetch_uv(t1i);
                    if (has_t2)
                        tc = fetch_uv(t2i);

                    tris.add(make_shared<triangle>(a, b, c, na, nb, nc, ta, tb, tc, mat));
                }
            }
        }

        auto bvh = make_shared<bvh_node>(tris);
        accel = bvh;
        bbox = bvh->bounding_box();
    }
};

#endif
