#include "sphere.h"
#include "hittable.h"
#include "hittable_list.h"
#include "raytracer.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "texture.h"
#include "quad.h"
#include "obj.h"
#include "constant_medium.h"

void depth_of_field_demo()
{
    hittable_list world;

    auto ground = make_shared<lambertian>(color(0.2, 1.0, 0.0));
    auto center = make_shared<lambertian>(color(0.9, 0.2, 0.2));
    auto metal_mat = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, ground));

    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, center));     // (focus this one)
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -0.8), 0.5, metal_mat)); // closer
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.8), 0.5, metal_mat));  // farther

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 25;
    cam.camera_center = point3(-2, 2, 1);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 10.0;
    cam.focus_dist = 3.4;

    cam.render(world);
}

void house_demo()
{
    hittable_list world;

    auto house_texture = make_shared<image_texture>("house_rgb.jpg");
    auto house_alpha = make_shared<image_texture>("house_alpha.jpg");
    auto house_mat = make_shared<alpha_lambertian>(house_texture, house_alpha, 1);
    auto house_model = make_shared<obj>("house.obj", house_mat);
    world.add(house_model);

    auto ground_mat = make_shared<lambertian>(color(0.0, 0.5804, 0.1255));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 25;
    cam.camera_center = point3(53, 1, 26);
    cam.set_from_euler(
        point3(17.0693, -36.4857, 8.76114), // Location
        vec3(82.7268, 0, 22.9697)           // Rotation
    );
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void bouncing_spheres()
{
    hittable_list world;

    auto checker = make_shared<solid_color>(color(1.0, 0.0, 0.0));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 1;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.camera_center = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}

void material_showcase()
{
    hittable_list world;

    // Ground
    auto ground_mat = make_shared<lambertian>(color(0.8, 0.8, 0.8));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100.0, ground_mat));

    // Sphere positions (left -> right)
    const double r = 0.5;
    point3 p0(-1.5, 0.0, -1.0);
    point3 p1(-0.5, 0.0, -1.0);
    point3 p2(0.5, 0.0, -1.0);
    point3 p3(1.5, 0.0, -1.0);

    // Materials
    auto diffuse_mat = make_shared<lambertian>(color(0.8, 0.2, 0.2));     // diffuse
    auto specular_mat = make_shared<metal>(color(0.8, 0.8, 0.8), 0.05);   // specular (metal)
    auto dielectric_mat = make_shared<dielectric>(1.5);                   // dielectric (glass)
    auto emissive_mat = make_shared<diffuse_light>(color(6.0, 6.0, 6.0)); // emissive

    // Spheres: diffuse, specular, dielectric, emissive
    world.add(make_shared<sphere>(p0, r, diffuse_mat));
    world.add(make_shared<sphere>(p1, r, specular_mat));
    world.add(make_shared<sphere>(p2, r, dielectric_mat));
    world.add(make_shared<sphere>(p3, r, emissive_mat));

    // Add a light so the non-emissive spheres are visible (area light above)
    auto light_mat = make_shared<diffuse_light>(color(4.0, 4.0, 4.0));
    world.add(make_shared<quad>(point3(-2, 2.5, -1.5), vec3(4, 0, 0), vec3(0, 0, 3), light_mat));

    // Optional BVH for consistency
    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 500;
    cam.max_depth = 100;

    cam.background = color(0.0, 0.0, 0.0);

    cam.vfov = 65;
    cam.camera_center = point3(0, 1.0, 2.5);
    cam.lookat = point3(0, 0, -1.0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void perlin_spheres()
{
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.camera_center = point3(0, 0, 4);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void final_render()
{
    hittable_list world;

    //--- Tree Model ---
    auto leafs_color = make_shared<image_texture>("leafs.jpg");
    auto leafs_opacity = make_shared<image_texture>("leafs_o.jpg");

    // Materials
    auto bark_mat = make_shared<lambertian>(make_shared<image_texture>("bark.jpg"));
    auto leafs_mat = make_shared<alpha_lambertian>(leafs_color, leafs_opacity, 0.5);

    // Models
    auto tree_model = make_shared<obj>("tree.obj", bark_mat);
    auto leafs_model = make_shared<obj>("leafs.obj", leafs_mat);

    world.add(tree_model);
    world.add(leafs_model);

    // --- Ornaments---
    const point3 cone_axis_center = point3(-0.1024, 0.0, -0.3769);

    const double bottom_y = 0.0;    // base of tree
    const double top_y = 20.0;      // tip height
    const double base_radius = 7.0; // radius at bottom_y

    const double y_min = 5.0;
    const double y_max = 20.0;

    const int ornament_count = 60;
    const double ornament_radius = 0.5;

    // Push ornaments slightly outward so they sit on the surface
    const double surface_push = 0.10;

    // Ornament materials
    auto red_metal = make_shared<metal>(color(0.9, 0.2, 0.1), 0.15);
    auto green_metal = make_shared<metal>(color(0.2, 0.9, 0.3), 0.15);
    auto gold_metal = make_shared<metal>(color(0.9, 0.75, 0.15), 0.05);
    auto blue_metal = make_shared<metal>(color(0.2, 0.4, 0.9), 0.10);

    auto pick_ornament_mat = [&]() -> shared_ptr<material>
    {
        double t = random_double();
        if (t < 0.25)
            return red_metal;
        if (t < 0.50)
            return green_metal;
        if (t < 0.75)
            return gold_metal;
        return blue_metal;
    };

    // Helper: cone radius at height y
    auto cone_radius_at = [&](double y)
    {
        double t = (y - bottom_y) / (top_y - bottom_y);
        t = interval(0, 1).clamp(t);
        return (1.0 - t) * base_radius;
    };

    for (int i = 0; i < ornament_count; i++)
    {
        double y = y_min + (y_max - y_min) * random_double();
        double r = cone_radius_at(y);

        double theta = 2.0 * pi * random_double();
        vec3 radial_dir = unit_vector(vec3(cos(theta), 0, sin(theta)));

        point3 p = point3(
            cone_axis_center.x + r * radial_dir.x,
            y,
            cone_axis_center.z + r * radial_dir.z);

        p += surface_push * radial_dir;

        p += vec3(0.10 * random_double(-1, 1),
                  0.10 * random_double(-1, 1),
                  0.10 * random_double(-1, 1));

        world.add(make_shared<sphere>(p, ornament_radius, pick_ornament_mat()));
    }

    // --- Christmas lights ---
    const int light_count = 260;
    const double light_radius = 0.08;
    const double light_surface_push = 0.18; // a bit more push so lights don't get buried
    const double light_y_min = 2.0;         // can start lower than ornaments
    const double light_y_max = 20.0;

    auto light_red = make_shared<diffuse_light>(color(8.0, 1.0, 1.0));
    auto light_green = make_shared<diffuse_light>(color(1.0, 8.0, 1.0));
    auto light_blue = make_shared<diffuse_light>(color(1.0, 1.0, 8.0));
    auto light_warm = make_shared<diffuse_light>(color(10.0, 6.0, 2.0));

    auto pick_light_mat = [&]() -> shared_ptr<material>
    {
        double t = random_double();
        if (t < 0.25)
            return light_red;
        if (t < 0.50)
            return light_green;
        if (t < 0.75)
            return light_blue;
        return light_warm;
    };

    for (int i = 0; i < light_count; i++)
    {
        // Slight bias upward so there are more lights higher up (optional)
        double y = light_y_min + (light_y_max - light_y_min) * std::sqrt(random_double());
        double r = cone_radius_at(y);

        double theta = 2.0 * pi * random_double();
        vec3 radial_dir = unit_vector(vec3(cos(theta), 0, sin(theta)));

        point3 p = point3(
            cone_axis_center.x + r * radial_dir.x,
            y,
            cone_axis_center.z + r * radial_dir.z);

        // Push outward more than ornaments
        p += light_surface_push * radial_dir;

        // Tiny jitter so they don't look like a perfect cone grid
        p += vec3(0.05 * random_double(-1, 1),
                  0.05 * random_double(-1, 1),
                  0.05 * random_double(-1, 1));

        world.add(make_shared<sphere>(p, light_radius, pick_light_mat()));
    }

    // --- Ground ---
    auto ground_mat = make_shared<lambertian>(color(1, 1, 1));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));

    // --- Presents---
    auto wrap_red = make_shared<lambertian>(color(0.85, 0.10, 0.10));
    auto wrap_green = make_shared<lambertian>(color(0.10, 0.70, 0.20));
    auto wrap_blue = make_shared<lambertian>(color(0.10, 0.25, 0.85));
    auto wrap_white = make_shared<lambertian>(color(0.95, 0.95, 0.95));
    auto wrap_gold = make_shared<metal>(color(0.90, 0.75, 0.20), 0.05);

    auto pick_wrap = [&]() -> shared_ptr<material>
    {
        double t = random_double();
        if (t < 0.20)
            return wrap_red;
        if (t < 0.40)
            return wrap_green;
        if (t < 0.60)
            return wrap_blue;
        if (t < 0.80)
            return wrap_white;
        return wrap_gold;
    };

    // Helper: make one present centered at 'c' with size 's' and yaw rotation.
    auto add_present = [&](const point3 &c, const vec3 &s, double yaw_deg)
    {
        // Build box in local space around origin, sitting on y=0
        point3 a(-0.5 * s.x, 0.0, -0.5 * s.z);
        point3 b(0.5 * s.x, s.y, 0.5 * s.z);

        auto gift = box(a, b, pick_wrap());

        // Rotate around Y, then translate into place
        shared_ptr<hittable> placed =
            make_shared<translate>(
                make_shared<rotate_y>(gift, yaw_deg),
                vec3(c.x, c.y, c.z));

        world.add(placed);
    };

    const int present_count = 140;
    const double ring_min = 5;
    const double ring_max = 30.0;

    for (int i = 0; i < present_count; i++)
    {
        double theta = 2.0 * pi * random_double();
        double r = ring_min + (ring_max - ring_min) * random_double();

        // Center around the tree base (your trunk center is cone_axis_center.xz)
        point3 c(
            cone_axis_center.x + r * std::cos(theta),
            0.0, // on ground
            cone_axis_center.z + r * std::sin(theta));

        // Random present dimensions
        vec3 s(
            random_double(0.8, 2.2),  // width (x)
            random_double(0.5, 1.8),  // height (y)
            random_double(0.8, 2.2)); // depth (z)

        double yaw = random_double(0.0, 360.0);

        add_present(c, s, yaw);
    }

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = "dusk";

    cam.vfov = 20;
    cam.camera_center = point3(53, 1, 26);
    cam.set_from_euler(
        point3(53, 1, 26),
        vec3(74, 0, 92));
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main()
{
    switch (1)
    {
    case 1:
        depth_of_field_demo();
        break;

    case 2:
        house_demo();
        break;

    case 3:
        bouncing_spheres();
        break;

    case 4:
        material_showcase();
        break;

    case 5:
        perlin_spheres();
        break;

    case 6:
        final_render();
        break;
    }

    return 0;
}
