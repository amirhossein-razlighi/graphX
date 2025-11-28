#include <cmath>

#include "tgaimage.h"
#include "line.h"

void line(int x0, int y0, int x1, int y1, TGAImage& frameBuffer, TGAColor colorToDrawLine) {
  const bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
  if (steep) {
    // steep slope, we transpose the line (x -> y and y -> x)
    std::swap(x0, y0);
    std::swap(x1, y1);
  }

  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  for (int x = x0; x <= x1; x++) {
    float t = static_cast<float>(x - x0) / static_cast<float>(x1 - x0);
    int y = static_cast<int>(std::round((1 - t) * y0 + t * y1));
    
    if (steep) {
      // if transposed, de-transpose 
      frameBuffer.set(y, x, colorToDrawLine);
      continue;
    }
    frameBuffer.set(x, y, colorToDrawLine);
  }
}