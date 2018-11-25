#include "Modules.h"

#include <cstdlib>
#include <string>

int main(int argc, char** argv)
{
    std::string image_path("image.dds");
    if (argc > 1) {
        image_path = argv[1];
    }

    const int image_width = 200;
    const int image_height = 100;
    gli::texture2d image(gli::format::FORMAT_RGB8_UINT_PACK8, gli::extent2d(image_width, image_height), 1);

    for (int y = 0; y < image_height; ++y) {
        int g = int((((image_height - 1) - y) / float(image_height - 1)) * 255.0f);
        for (int x = 0; x < image_width; ++x) {
            int r = int((((image_width - 1) - x) / float(image_width - 1)) * 255.0f);
            image.store(gli::extent2d(x, y), 0, glm::u8vec3(r, g, 51));
        }
    }

    gli::save_dds(image, image_path);
    return (EXIT_SUCCESS);
}
