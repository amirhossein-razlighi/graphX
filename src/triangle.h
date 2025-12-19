#pragma once
#include "tgaimage.h"
class Triangle
{
public:
  std::vector<std::tuple<int, int, int>> vertices;
  int x0, y0, x1, y1, x2, y2;
  double z0, z1, z2;
  TGAColor color0, color1, color2;
  std::pair<int, int> bbox_min_point;
  std::pair<int, int> bbox_max_point;

  Triangle(int x0, int y0, double z0, int x1, int y1, double z1, int x2, int y2, double z2, TGAColor color0, TGAColor color1, TGAColor color2)
      : color0(color0), color1(color1), color2(color2), x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2)
  {
    vertices.push_back({x0, y0, z0});
    vertices.push_back({x1, y1, z1});
    vertices.push_back({x2, y2, z2});
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
  double area() const
  {
    return triangleSignedArea(x0, y0, x1, y1, x2, y2);
  }

  TGAColor interpolateColor(double alpha, double beta, double gamma) const
  {
    TGAColor c0 = color0;
    TGAColor c1 = color1;
    TGAColor c2 = color2;
    TGAColor interpolated;

    for (int i = 0; i < 4; i++)
    {
      interpolated[i] = static_cast<unsigned char>(alpha * c0[i] + beta * c1[i] + gamma * c2[i]);
    }
    return interpolated;
  }

  double getDepthAtBarycentric(double alpha, double beta, double gamma) const
  {
    return alpha * z0 + beta * z1 + gamma * z2;
  }

private:
  void computeBoundingBox();
};