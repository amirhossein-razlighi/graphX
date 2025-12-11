#include "tgaimage.h"
#include "renderer.h"
#include "triangle.h"

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

int main(int argc, char **argv)
{
    constexpr int width = 128;
    constexpr int height = 128;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    const Triangle tri(10, 20, 80, 30, 50, 90, red);
    Renderer renderer(framebuffer);
    renderer.drawFilledBoundingBox(tri, blue);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}