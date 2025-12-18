#include "renderer.h"

void Renderer::drawLine(int x0, int y0, int x1, int y1, TGAColor color)
{
    const bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
    if (steep)
    {
        // steep slope, we transpose the line (x -> y and y -> x)
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; x++)
    {
        float t = static_cast<float>(x - x0) / static_cast<float>(x1 - x0);
        int y = static_cast<int>(std::round((1 - t) * y0 + t * y1));

        if (steep)
        {
            // if transposed, de-transpose
            framebuffer.set(y, x, color);
            continue;
        }
        framebuffer.set(x, y, color);
    }
}
void Renderer::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color)
{
    this->drawLine(x0, y0, x1, y1, color);
    this->drawLine(x1, y1, x2, y2, color);
    this->drawLine(x2, y2, x0, y0, color);
}
void Renderer::drawFilledTriangleLegacy(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color)
{
    // Sort vertices by y-coordinate
    if (y0 > y1)
        std::swap(y0, y1), std::swap(x0, x1);
    if (y0 > y2)
        std::swap(y0, y2), std::swap(x0, x2);
    if (y1 > y2)
        std::swap(y1, y2), std::swap(x1, x2);

    int total_height = y2 - y0;

    if (y0 != y2)
    {
        int segment_height = y1 - y0;

        for (int y = y0; y < y1; ++y)
        {
            int x_a = x0 + ((x2 - x0) * (y - y0)) / total_height;
            int x_b = x0 + ((x1 - x0) * (y - y0)) / segment_height;
            this->drawLine(x_a, y, x_b, y, color);
        }
    }

    if (y1 != y2)
    {
        int segment_height = y2 - y1;

        for (int y = y1; y < y2; ++y)
        {
            int x_a = x0 + ((x2 - x0) * (y - y0)) / total_height;
            int x_b = x1 + ((x2 - x1) * (y - y1)) / segment_height;
            this->drawLine(x_a, y, x_b, y, color);
        }
    }
}
void Renderer::drawBoundingBox(const Triangle &tri, TGAColor color)
{
    int min_x = tri.bbox_min_point.first;
    int min_y = tri.bbox_min_point.second;
    int max_x = tri.bbox_max_point.first;
    int max_y = tri.bbox_max_point.second;

    this->drawLine(min_x, min_y, max_x, min_y, color); // Top edge
    this->drawLine(max_x, min_y, max_x, max_y, color); // Right edge
    this->drawLine(max_x, max_y, min_x, max_y, color); // Bottom edge
    this->drawLine(min_x, max_y, min_x, min_y, color); // Left edge
}

void Renderer::drawFilledTriangle(const Triangle &tri)
{
    int min_x = tri.bbox_min_point.first;
    int min_y = tri.bbox_min_point.second;
    int max_x = tri.bbox_max_point.first;
    int max_y = tri.bbox_max_point.second;

    #pragma omp parallel for
    for (int x = min_x; x <= max_x; ++x)
    {
        for (int y = min_y; y <= max_y; ++y)
        {
            // Computing the baryCentric coordinates
            std::tuple<double, double, double> baryCoords = tri.barycentricCoordinates(x, y);
            double alpha = std::get<0>(baryCoords);
            double beta = std::get<1>(baryCoords);
            double gamma = std::get<2>(baryCoords);
            if (alpha < 0 || beta < 0 || gamma < 0)
                continue; // Point is outside the triangle
            TGAColor interpolated_color = tri.interpolateColor(alpha, beta, gamma);
            unsigned char depth = tri.getDepthAtBarycentric(alpha, beta, gamma);
            
            if (depth > zbuffer.get(x, y)[0])
            {
                framebuffer.set(x, y, interpolated_color);
                zbuffer.set(x, y, {depth});
            }
        }
    }
}