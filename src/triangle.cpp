#include "triangle.h"
#include "line.h"

void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color)
{
  line(x0, y0, x1, y1, image, color);
  line(x1, y1, x2, y2, image, color);
  line(x2, y2, x0, y0, image, color);
}