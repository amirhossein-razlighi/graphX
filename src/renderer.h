#pragma once
#include "tgaimage.h"
#include "triangle.h"

class Renderer
{
public:
    TGAImage &framebuffer;
    Renderer(TGAImage &fb) : framebuffer(fb) {}
    void drawLine(int x0, int y0, int x1, int y1, TGAColor color);
    void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color);
    void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color);
    void drawBoundingBox(const Triangle &tri, TGAColor color);
    void drawFilledBoundingBox(const Triangle &tri, TGAColor color);
};