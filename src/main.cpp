#include "geometry.h"
#include "model.h"
#include "tgaimage.h"
#include "triangle.h"
#include "renderer.h"

mat<4, 4> ModelView, Viewport, Perspective;

void lookat(const vec3 eye, const vec3 center, const vec3 up)
{
    vec3 n = normalized(eye - center);
    vec3 l = normalized(cross(up, n));
    vec3 m = normalized(cross(n, l));
    ModelView = mat<4, 4>{{{l.x, l.y, l.z, 0}, {m.x, m.y, m.z, 0}, {n.x, n.y, n.z, 0}, {0, 0, 0, 1}}} *
                mat<4, 4>{{{1, 0, 0, -center.x}, {0, 1, 0, -center.y}, {0, 0, 1, -center.z}, {0, 0, 0, 1}}};
}

void perspective(const double f)
{
    Perspective = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, -1 / f, 1}}};
}

void viewport(const int x, const int y, const int w, const int h)
{
    Viewport = {{{w / 2., 0, 0, x + w / 2.}, {0, h / 2., 0, y + h / 2.}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

void rasterize(const vec4 clip[3], Renderer &renderer, const TGAColor color)
{
    vec4 ndc[3] = {clip[0] / clip[0].w, clip[1] / clip[1].w, clip[2] / clip[2].w};                   // normalized device coordinates
    vec2 screen[3] = {(Viewport * ndc[0]).xy(), (Viewport * ndc[1]).xy(), (Viewport * ndc[2]).xy()}; // screen coordinates

    mat<3, 3> ABC = {{{screen[0].x, screen[0].y, 1.}, {screen[1].x, screen[1].y, 1.}, {screen[2].x, screen[2].y, 1.}}};
    if (ABC.det() < 1)
        return; // backface culling + discarding triangles that cover less than a pixel

    Triangle tri(
        static_cast<int>(screen[0].x), static_cast<int>(screen[0].y), (ndc[0].z),
        static_cast<int>(screen[1].x), static_cast<int>(screen[1].y), (ndc[1].z),
        static_cast<int>(screen[2].x), static_cast<int>(screen[2].y), (ndc[2].z),
        color, color, color);

    renderer.drawFilledTriangle(tri);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }

    constexpr int width = 800; // output image size
    constexpr int height = 800;
    const vec3 eye{2, 0, 10};   // camera position
    const vec3 center{0, 0, 0}; // camera direction
    const vec3 up{0, 1, 0};     // camera up vector

    lookat(eye, center, up);
    perspective(norm(eye - center));
    viewport(width / 16, height / 16, width * 7 / 8, height * 7 / 8);

    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());
    Renderer renderer(framebuffer, zbuffer);

    for (int m = 1; m < argc; m++)
    { // iterate through all input objects
        Model model(argv[m]);
        for (int i = 0; i < model.nfaces(); i++)
        { // iterate through all triangles
            vec4 clip[3];
            for (int d : {0, 1, 2})
            { // assemble the primitive
                vec3 v = model.vert(i, d);
                clip[d] = Perspective * ModelView * vec4{v.x, v.y, v.z, 1.};
            }
            TGAColor rnd;
            for (int c = 0; c < 3; c++)
                rnd[c] = std::rand() % 255;
            rasterize(clip, renderer, rnd); // rasterize the primitive
        }
    }
    renderer.renderZBuffer("zbuffer.tga");
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}