#include "triangle.h"

void Triangle::computeBoundingBox()
{
  int min_x = std::min(std::min(x0, x1), x2);
  int min_y = std::min(std::min(y0, y1), y2);
  int max_x = std::max(std::max(x0, x1), x2);
  int max_y = std::max(std::max(y0, y1), y2);
  bbox_min_point = {min_x, min_y};
  bbox_max_point = {max_x, max_y};
}