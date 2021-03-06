#include "Modules.h"

#include <cstdlib>
#include <string>

int main(int argc, char** argv)
{
    std::string imagePath("OutputAnImage.dds");
    if (argc > 1) {
        imagePath = argv[1];
    }

    const int imageWidth = 200;
    const int imageHeight = 100;
    gli::texture2d image(gli::format::FORMAT_BGR8_UNORM_PACK8, gli::extent2d(imageWidth, imageHeight), 1);
    const int scanlineOrder = imageHeight - 1;

    for (int y = 0; y < imageHeight; ++y) {
        int g = int((float(y) / float(imageHeight - 1)) * 255.0f);
        for (int x = 0; x < imageWidth; ++x) {
            int r = int((float(x) / float(imageWidth - 1)) * 255.0f);
            image.store(gli::extent2d(x, scanlineOrder - y), 0, glm::u8vec3(51, g, r));
        }
    }

    gli::save_dds(image, imagePath);
    return (EXIT_SUCCESS);
}
