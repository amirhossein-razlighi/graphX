#include <iostream>
#include "tgaimage.h"
#include "renderer.h"
#include "triangle.h"
#include "model.h"

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }

    Model model(argv[1]);
    const int IMAGE_WIDTH = 800;
    const int IMAGE_HEIGHT = 800;
    TGAImage frameBuffer(IMAGE_WIDTH, IMAGE_HEIGHT, TGAImage::RGB);
    Renderer renderer(frameBuffer);

    for (int i = 0; i < model.nfaces(); i++)
    {
        vec3 v0 = model.vert(i, 0);
        vec3 v1 = model.vert(i, 1);
        vec3 v2 = model.vert(i, 2);

        // Transform from normalized device coordinates to screen coordinates
        int x0 = static_cast<int>((v0.x + 1.0f) * IMAGE_WIDTH / 2.0f);
        int y0 = static_cast<int>((v0.y + 1.0f) * IMAGE_HEIGHT / 2.0f);
        int x1 = static_cast<int>((v1.x + 1.0f) * IMAGE_WIDTH / 2.0f);
        int y1 = static_cast<int>((v1.y + 1.0f) * IMAGE_HEIGHT / 2.0f);
        int x2 = static_cast<int>((v2.x + 1.0f) * IMAGE_WIDTH / 2.0f);
        int y2 = static_cast<int>((v2.y + 1.0f) * IMAGE_HEIGHT / 2.0f);

        TGAColor random_color_v0;
        TGAColor random_color_v1;
        TGAColor random_color_v2;
        for (int c = 0; c < 3; c++)
        {
            random_color_v0[c] = static_cast<std::uint8_t>(std::rand() % 256);
            random_color_v1[c] = static_cast<std::uint8_t>(std::rand() % 256);
            random_color_v2[c] = static_cast<std::uint8_t>(std::rand() % 256);
        }

        Triangle tri(x0, y0, x1, y1, x2, y2, random_color_v0, random_color_v1, random_color_v2);
        renderer.drawFilledTriangle(tri);
    }

    frameBuffer.write_tga_file("framebuffer.tga");

    return 0;
}