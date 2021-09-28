#include "renderer.h"

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

Render::Render(std::shared_ptr<hittable_list> world, double aspect_ratio, int image_width, int spp,
            color background, point3 lookfrom, point3 lookat, double vfov, vec3 vup,
            double aperture, double dist_to_focus, RenderType renderType)
{
    if (renderType == PT)
    {
        m_integrator = std::make_shared<PTIntegrator>();
    }
    else if (renderType == VolPT)
    {
        m_integrator = std::make_shared<VPTIntegrator>();
    }
    
    m_world = world;
    m_aspect_ratio = aspect_ratio;
    m_image_width = image_width;
    m_spp = spp;
    m_background = background;
    m_lookfrom = lookfrom;
    m_lookat = lookat;
    m_vfov = vfov;
    m_image_height = static_cast<int>(m_image_width / m_aspect_ratio);

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    m_cam = &cam;
}

void Render::Start()
{
    std::cout << "P3\n"
              << m_image_width << ' ' << m_image_height << "\n255\n";

    for (int j = m_image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < m_image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < m_spp; ++s)
            {
                auto u = (i + random_double()) / (m_image_width - 1);
                auto v = (j + random_double()) / (m_image_height - 1);
                ray r = m_cam->get_ray(u, v);
                pixel_color += m_integrator->castRay(r, m_background, m_world, 0);
            }
            write_color(std::cout, pixel_color, m_spp);
        }
    }

    std::cerr << "\nDone.\n";
}