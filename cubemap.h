#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "raytracer.h"
#include "rtw_stb_image.h"

#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

class cubemap
{
public:
    explicit cubemap(const std::string &name) { load_from_name(name); }

    bool is_valid() const { return valid; }

    // Sample the environment in direction "dir" (world-space ray direction).
    color sample(const vec3 &dir) const
    {
        if (!valid)
            return color(1, 0, 1); // debug magenta

        vec3 d = unit_vector(dir);

        double ax = std::fabs(d.x), ay = std::fabs(d.y), az = std::fabs(d.z);

        int face = 0;
        double u = 0, v = 0;

        // Face selection by dominant axis (OpenGL-style cube map convention).
        if (ax >= ay && ax >= az)
        {
            // +/-X
            if (d.x > 0)
            {
                face = POS_X;
                u = -d.z / ax;
                v = d.y / ax;
            }
            else
            {
                face = NEG_X;
                u = d.z / ax;
                v = d.y / ax;
            }
        }
        else if (ay >= ax && ay >= az)
        {
            // +/-Y
            if (d.y > 0)
            {
                face = POS_Y;
                u = d.x / ay;
                v = -d.z / ay;
            }
            else
            {
                face = NEG_Y;
                u = d.x / ay;
                v = d.z / ay;
            }
        }
        else
        {
            // +/-Z
            if (d.z > 0)
            {
                face = POS_Z;
                u = d.x / az;
                v = d.y / az;
            }
            else
            {
                face = NEG_Z;
                u = -d.x / az;
                v = d.y / az;
            }
        }

        // Map from [-1,1] to [0,1]
        u = 0.5 * (u + 1.0);
        v = 0.5 * (v + 1.0);

        // Match your image_texture convention (flip V)
        v = 1.0 - v;

        const int w = faces[face].width();
        const int h = faces[face].height();
        if (w <= 0 || h <= 0)
            return color(1, 0, 1);

        int i = static_cast<int>(u * w);
        int j = static_cast<int>(v * h);

        const unsigned char *p = faces[face].pixel_data(i, j);
        const double s = 1.0 / 255.0;
        return color(s * p[0], s * p[1], s * p[2]);
    }

private:
    enum FaceIndex
    {
        POS_X = 0,
        NEG_X = 1,
        POS_Y = 2,
        NEG_Y = 3,
        POS_Z = 4,
        NEG_Z = 5
    };

    std::array<rtw_image, 6> faces;
    bool valid = false;

    static bool try_load_with_exts(rtw_image &img, const std::string &no_ext_path)
    {
        static const char *exts[] = {".png", ".jpg", ".jpeg", ".bmp", ".tga", ".hdr"};
        for (const char *ext : exts)
        {
            if (img.load(no_ext_path + ext))
                return true;
        }
        // Also allow the user to include extension in the name (rare, but harmless)
        if (img.load(no_ext_path))
            return true;
        return false;
    }

    bool load_face(FaceIndex idx, const std::vector<std::string> &candidate_no_ext_paths)
    {
        for (const auto &p : candidate_no_ext_paths)
        {
            if (try_load_with_exts(faces[idx], p))
                return true;
        }
        return false;
    }

    void load_from_name(const std::string &name)
    {
        const std::string root = "cubemaps/" + name + "/";

        auto paths = [&](const std::string &base)
        {
            return std::vector<std::string>{root + base};
        };

        bool ok = true;
        ok &= load_face(POS_X, paths("posx"));
        ok &= load_face(NEG_X, paths("negx"));
        ok &= load_face(POS_Y, paths("posy"));
        ok &= load_face(NEG_Y, paths("negy"));

        // Support both "posz" and your "poz" spelling
        {
            auto posz = paths("posz");
            auto poz = paths("poz");
            posz.insert(posz.end(), poz.begin(), poz.end());
            ok &= load_face(POS_Z, posz);
        }

        ok &= load_face(NEG_Z, paths("negz"));

        valid = ok;
        if (!valid)
        {
            std::cerr << "ERROR: Could not load cubemap '" << name
                      << "' from ./cubemaps/" << name
                      << "/. Check filenames and working directory.\n";
        }
    }
};

#endif
