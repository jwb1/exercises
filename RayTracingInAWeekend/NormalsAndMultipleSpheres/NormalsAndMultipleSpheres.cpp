#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <string>
#include <vector>

#include "Modules.h"
#include "Ray.h"
#include "Intersection.h"
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
    std::string imagePath("NormalsAndMultipleSpheres.dds");
    if (argc > 1) {
        imagePath = argv[1];
    }

    // Render target.
    const int imageWidth = 200;
    const int imageHeight = 100;
    gli::texture2d image(gli::format::FORMAT_BGR8_UNORM_PACK8, gli::extent2d(imageWidth, imageHeight), 1);
    const int scanlineOrder = imageHeight - 1;

    // 3D camera parameters.
    glm::vec3 origin(0.0f, 0.0f, 0.0f);
    float nearDistance = -1.0f;
    float aspectRatio = float(imageWidth) / float(imageHeight);
    float fieldOfView = 0.927295218f; // radians. About 53 degrees. Chosen so the near rect height is 1.
                                      // At a 2/1 aspect ratio, this gives a horizontal fov of 90 degrees.

    float nearHeight = 2.0f * std::fabs(nearDistance) * std::tanf(fieldOfView / 2.0f);
    float nearWidth = nearHeight * aspectRatio;
    glm::vec3 nearLowerLeft = origin + glm::vec3(-(nearWidth / 2.0f), -(nearHeight / 2.0f), nearDistance);

    // Scene is made of solids.
    SolidList solids;
    solids.AddSolid(new Sphere(glm::vec3(0.0f, 0.0f, -1.5f), 0.5f));
    solids.AddSolid(new Sphere(glm::vec3(0.0f, -100.5f, -1.5f), 100.0f));

    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            glm::vec2 pixelPosition(float(x) / float(imageWidth - 1), float(y) / float(imageHeight - 1));

            Ray eyeRay(origin, nearLowerLeft + glm::vec3(pixelPosition.x * nearWidth, pixelPosition.y * nearHeight, 0.0f));
            glm::vec3 color = TraceRay(eyeRay, solids);

            // Swizzle and pack the color and store.
            glm::u8vec3 packedColor(int(color.b * 255.0f), int(color.g * 255.0f), int(color.r * 255.0f));
            image.store(gli::extent2d(x, scanlineOrder - y), 0, packedColor);
        }
    }

    gli::save_dds(image, imagePath);
    return (EXIT_SUCCESS);
}
