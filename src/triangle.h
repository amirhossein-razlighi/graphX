#pragma once
#include "tgaimage.h"

class Triangle
{
public:
  std::vector<std::pair<int, int>> vertices;
  int x0, y0, x1, y1, x2, y2;
  TGAColor color;
  std::pair<int, int> bbox_min_point;
  std::pair<int, int> bbox_max_point;

  Triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor col)
      : color(col), x0(x0), y0(y0), x1(x1), y1(y1), x2(x2), y2(y2)
  {
    vertices.push_back({x0, y0});
    vertices.push_back({x1, y1});
    vertices.push_back({x2, y2});
    computeBoundingBox();
  }
  static double triangleSignedArea(int x0, int y0, int x1, int y1, int x2, int y2)
  {
    return 0.5 * ((x0 * y1) - (x1 * y0) + (x1 * y2) - (x2 * y1) + (x2 * y0) - (x0 * y2));
  }

  std::tuple<double, double, double> barycentricCoordinates(int px, int py) const
  {
    double A = area();
    double alpha = triangleSignedArea(px, py, x1, y1, x2, y2) / A;
    double beta = triangleSignedArea(x0, y0, px, py, x2, y2) / A;
    double gamma = triangleSignedArea(x0, y0, x1, y1, px, py) / A;
    return std::make_tuple(alpha, beta, gamma);
  }

private:
  void computeBoundingBox();
  double area() const
  {
    return triangleSignedArea(x0, y0, x1, y1, x2, y2);
  }
};