#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

#include "Modules.h"
#include "Random.h"
#include "Ray.h"
#include "Intersection.h"
#include "Camera.h"
#include "LambertianMaterial.h"
#include "MetalMaterial.h"
#include "DielectricMaterial.h"
#include "Solid.h"
#include "Sphere.h"

class Dielectrics {
public:
    Dielectrics(int argc, char** argv)
        : m_imagePath("Dielectrics.dds")
        , m_image(m_imageFormat, gli::extent2d(m_imageWidth, m_imageHeight), 1)
        , m_camera(
            glm::vec3(0.0f, 0.0f, 0.0f),  // Location of camera
            glm::vec3(0.0f, 0.0f, -1.0f), // Looking down z axis.
            glm::vec3(0.0f, 1.0f, 0.0f),  // Positive Y is up.
            -1.0f,                        // Distance to image (along lookAt) (TODO! Should be positive!)
            m_imageWidth,
            m_imageHeight,
            0.927295218f                  // FOV is about 53 degrees. Chosen so the image rect height is 1.
                                          // At a 2/1 aspect ratio, this gives a horizontal fov of 90 degrees.
            )
    {
        if (argc > 1) {
            m_imagePath = argv[1];
        }

        // Scene definition:
        m_solids.AddSolid(new Sphere(glm::vec3(0.0f, -100.5f, -1.5f), 100.0f, new LambertianMaterial(glm::vec3(0.8f, 0.8f, 0.0f))));

        m_solids.AddSolid(new Sphere(glm::vec3(-1.0f, 0.0f, -1.5f), 0.5f, new LambertianMaterial(glm::vec3(0.1f, 0.2f, 0.5f))));
        m_solids.AddSolid(new Sphere(glm::vec3( 1.0f, 0.0f, -1.5f), 0.5f, new MetalMaterial(glm::vec3(0.8f, 0.6f, 0.2f))));
        m_solids.AddSolid(new Sphere(glm::vec3( 0.0f, 0.0f, -1.5f), 0.5f, new DielectricMaterial(1.5f)));
    }

    int Render()
    {
        // Sample positions are in lower left corner of the region to be sampled for
        // a pixel. 0-1 allows us to sample the entire region.
        std::uniform_real_distribution<float> samplePositionDistribution(0.0f, 1.0f);

        // Compute each pixel value.
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime(std::chrono::high_resolution_clock::now());
        int progressReport = m_imageHeight / 10;
        for (int y = 0; y < m_imageHeight; ++y) {
            for (int x = 0; x < m_imageWidth; ++x) {
                //if (x == 26 && y == 50) __debugbreak();

                glm::vec3 color(0.0f, 0.0f, 0.0f);

                for (int s = 0; s < m_numberOfSamples; ++s) {
                    glm::vec2 samplePosition(
                        (float(x) + samplePositionDistribution(GetRandomDevice())) / float(m_imageWidth - 1),
                        (float(y) + samplePositionDistribution(GetRandomDevice())) / float(m_imageHeight - 1)
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

            if (y % progressReport == 0) {
                std::cout << "Rendering is " << (float(y) / float(m_imageHeight) * 100.0f) << "% done." << std::endl;
            }
        }
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime(std::chrono::high_resolution_clock::now());

        std::cout
            << "Rendering is complete. Writing " << m_imagePath << ". Render Time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
            << "ms." << std::endl;;

        gli::save_dds(m_image, m_imagePath);
        return (EXIT_SUCCESS);
    }

private:
    glm::vec3 TraceRay(const Ray& ray, int recursionDepth = 0) const
    {
        Intersection intersection;
        if (m_solids.Intersect(ray, 0.001f, FLT_MAX, intersection)) {
            // Interaction between the intersecting light and solid material.
            Ray scatteredRay;
            glm::vec3 attenuation;
            if (recursionDepth < m_maxRecursion && intersection.material->Scatter(ray, intersection, attenuation, scatteredRay)) {
                return (attenuation * TraceRay(scatteredRay, recursionDepth + 1));
            }
            else {
                return (glm::vec3(0.0f, 0.0f, 0.0f));
            }
        }
        else {
            // Background is just a lerp between white and light blue on y position.
            float t = ray.Direction().y + 0.5f; // Assumes y goes from (-0.5, 0.5)
            return (((1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f)) + (t * glm::vec3(0.5f, 0.7f, 1.0f)));
        }
    }

    // Render Target.
    static constexpr int m_imageWidth = 200;
    static constexpr int m_imageHeight = 100;
    static constexpr gli::format m_imageFormat = gli::format::FORMAT_RGBA8_SRGB_PACK8;
    static constexpr int m_scanlineOrder = m_imageHeight - 1;
    std::string m_imagePath;
    gli::texture2d m_image;

    // 3D camera.
    Camera m_camera;

    // Scene is made of solids.
    SolidList m_solids;

    // Samples per pixel
    static constexpr int m_numberOfSamples = 100;

    // Ray trace recursion limit.
    static constexpr int m_maxRecursion = 50;
};

int main(int argc, char** argv)
{
    Dielectrics renderer(argc, argv);
    return (renderer.Render());
}
