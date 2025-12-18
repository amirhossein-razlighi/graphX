#pragma once
#include "tgaimage.h"
#include "triangle.h"

class Renderer
{
public:
    TGAImage &framebuffer;
    TGAImage &zbuffer;
    Renderer(TGAImage &fb, TGAImage &zb) : framebuffer(fb), zbuffer(zb) {}
    void drawLine(int x0, int y0, int x1, int y1, TGAColor color);
    void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color);\
    void drawFilledTriangle(const Triangle &tri);
    void drawFilledTriangleLegacy(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color);
    void drawBoundingBox(const Triangle &tri, TGAColor color);
};