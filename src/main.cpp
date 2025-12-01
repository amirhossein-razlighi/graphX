#include "tgaimage.h"
#include "line.h"
#include "draw_obj.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

const std::string filename = "./diablo3_pose.obj";

int main(int argc, char** argv) {
    constexpr int width  = 512;
    constexpr int height = 512;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    drawWireframe(filename, framebuffer, white);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}