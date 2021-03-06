#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <string>
#include <vector>
#include <random>

#include "Modules.h"
#include "Random.h"
#include "Ray.h"
#include "Intersection.h"
#include "Camera.h"
#include "Solid.h"
#include "Sphere.h"

glm::vec3 TraceRay(const Ray& ray, const Solid& solid)
{
    Intersection intersection;
    if (solid.Intersect(ray, 0.0f, FLT_MAX, intersection)) {
        // Components of the normal vector are in the range (-1.0, 1.0). Rescale to (0, 1.0)
        return ((intersection.normal + glm::vec3(1.0f, 1.0f, 1.0f)) / 2.0f);
    }
    else {
        // Background is just a lerp between white and light blue on y position.
        float t = ray.Direction().y + 0.5f; // Assumes y goes from (-0.5, 0.5)
        return (((1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f)) + (t * glm::vec3(0.5f, 0.7f, 1.0f)));
    }
}

int main(int argc, char** argv)
{
    std::string imagePath("Antialiasing.dds");
    if (argc > 1) {
        imagePath = argv[1];
    }

    // Random number distributrion
    std::uniform_real_distribution<float> samplePositionDistribution(0.0f, 1.0f);

    // Render target.
    const int imageWidth = 200;
    const int imageHeight = 100;
    gli::texture2d image(gli::format::FORMAT_BGR8_UNORM_PACK8, gli::extent2d(imageWidth, imageHeight), 1);
    const int scanlineOrder = imageHeight - 1;

    // 3D camera parameters.
    Camera camera(
        glm::vec3(0.0f, 0.0f, 0.0f),  // Location of camera
        glm::vec3(0.0f, 0.0f, -1.0f), // Looking down z axis.
        glm::vec3(0.0f, 1.0f, 0.0f),  // Positive Y is up.
        -1.0f,                        // Distance to image (along lookAt) (TODO! Should be positive!)
        imageWidth,
        imageHeight,
        0.927295218f                 // FOV is about 53 degrees. Chosen so the image rect height is 1.
                                     // At a 2/1 aspect ratio, this gives a horizontal fov of 90 degrees.
        );

    // Scene is made of solids.
    SolidList solids;
    solids.AddSolid(new Sphere(glm::vec3(0.0f, 0.0f, -1.5f), 0.5f));
    solids.AddSolid(new Sphere(glm::vec3(0.0f, -100.5f, -1.5f), 100.0f));

    // Compute each pixel value.
    const int numberOfSamples = 100;
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            glm::vec3 color(0.0f, 0.0f, 0.0f);

            for (int s = 0; s < numberOfSamples; ++s) {
                glm::vec2 samplePosition(
                    (float(x) + samplePositionDistribution(GetRandomDevice())) / float(imageWidth - 1),
                    (float(y) + samplePositionDistribution(GetRandomDevice())) / float(imageHeight - 1)
                );

                Ray eyeRay(camera.MakeEyeRay(samplePosition));
                color += TraceRay(eyeRay, solids);
            }

            color /= numberOfSamples;

            // Swizzle and pack the color and store.
            glm::u8vec3 packedColor(int(color.b * 255.0f), int(color.g * 255.0f), int(color.r * 255.0f));
            image.store(gli::extent2d(x, scanlineOrder - y), 0, packedColor);
        }
    }

    gli::save_dds(image, imagePath);
    return (EXIT_SUCCESS);
}
