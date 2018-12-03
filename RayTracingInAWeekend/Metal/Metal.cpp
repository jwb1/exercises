#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <string>
#include <vector>
#include <random>

#include "Modules.h"
#include "Ray.h"
#include "Intersection.h"
#include "Camera.h"
#include "Material.h"
#include "LambertianMaterial.h"
#include "Solid.h"
#include "Sphere.h"

class Metal {
public:
    Metal(int argc, char** argv)
        : m_randomDevice() 
        , m_mtDevice(m_randomDevice())
        , m_imagePath("Metal.dds")
        , m_image(m_imageFormat, gli::extent2d(m_imageWidth, m_imageHeight), 1)
        , m_camera(-1.0f, m_imageWidth, m_imageHeight, 0.927295218f)
    {
        if (argc > 1) {
            m_imagePath = argv[1];
        }

        // Render two spheres. One entirely in camera, and one partially
        m_solids.AddSolid(new Sphere(glm::vec3(0.0f, 0.0f, -1.5f), 0.5f));
        m_solids.AddSolid(new Sphere(glm::vec3(0.0f, -100.5f, -1.5f), 100.0f));
    }

    int Render()
    {
        // Sample positions are in lower left corner of the region to be sampled for
        // a pixel. 0-1 allows us to sample the entire region.
        std::uniform_real_distribution<float> samplePositionDistribution(0.0f, 1.0f);

        // Compute each pixel value.
        for (int y = 0; y < m_imageHeight; ++y) {
            for (int x = 0; x < m_imageWidth; ++x) {
                glm::vec3 color(0.0f, 0.0f, 0.0f);

                for (int s = 0; s < m_numberOfSamples; ++s) {
                    glm::vec2 samplePosition(
                        (float(x) + samplePositionDistribution(m_mtDevice)) / float(m_imageWidth - 1),
                        (float(y) + samplePositionDistribution(m_mtDevice)) / float(m_imageHeight - 1)
                    );

                    Ray eyeRay(m_camera.MakeEyeRay(samplePosition));
                    color += TraceRay(eyeRay);
                }

                color /= m_numberOfSamples;

                // Convert to sRGB
                glm::vec3 srgbColor(glm::convertLinearToSRGB(color));

                // Swizzle and pack the color and store.
                glm::u8vec4 packedColor(int(srgbColor.r * 255.0f), int(srgbColor.g * 255.0f), int(srgbColor.b * 255.0f), 255);
                m_image.store(gli::extent2d(x, m_scanlineOrder - y), 0, packedColor);
            }
        }

        gli::save_dds(m_image, m_imagePath);
        return (EXIT_SUCCESS);
    }

private:
    glm::vec3 TraceRay(const Ray& ray) const
    {
        Intersection intersection;
        if (m_solids.Intersect(ray, 0.001f, FLT_MAX, intersection)) {
            // Interaction between the intersecting light and solid material.

        }
        else {
            // Background is just a lerp between white and light blue on y position.
            float t = ray.Direction().y + 0.5f; // Assumes y goes from (-0.5, 0.5)
            return (((1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f)) + (t * glm::vec3(0.5f, 0.7f, 1.0f)));
        }
    }

    // Random number generator.
    std::random_device m_randomDevice;
    mutable std::mt19937 m_mtDevice;

    // Render Target
    static constexpr int m_imageWidth = 200;
    static constexpr int m_imageHeight = 100;
    static constexpr gli::format m_imageFormat = gli::format::FORMAT_RGBA8_SRGB_PACK8;
    static constexpr int m_scanlineOrder = m_imageHeight - 1;
    std::string m_imagePath;
    gli::texture2d m_image;

    // 3D camera parameters.
    // FOV is about 53 degrees. Chosen so the image rect height is 1.
    // At a 2/1 aspect ratio, this gives a horizontal fov of 106 degrees.
    Camera m_camera;

    // Scene is made of solids.
    SolidList m_solids;

    static constexpr int m_numberOfSamples = 100;
};

int main(int argc, char** argv)
{
    Metal renderer(argc, argv);
    return (renderer.Render());
}
