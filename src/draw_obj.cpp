#include "draw_obj.h"
#include "line.h"

void drawWireframe(const std::string &filename, TGAImage &frameBuffer, TGAColor lineColor)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  // read line by line. Each line that starts with 'v' is a vertex
  // store it. Each line starts with 'f' is a face, draw lines between vertices
  // like this: f 1/2/3 4/5/6 7/8/9
  // means drawing line between 1 and 2 and 3 (vertex indices)
  // and 4 and 5 and 6, and so on.
  std::string str_line;
  std::vector<std::vector<float>> vertices;
  int faces_processed = 0;

  while (std::getline(file, str_line))
  {
    if (str_line.substr(0, 2) == "v ")
    {
      std::istringstream iss(str_line.substr(2));
      std::vector<float> vertex;
      float x, y, z;
      iss >> x >> y >> z;
      vertex.push_back(x);
      vertex.push_back(y);
      vertex.push_back(z);
      vertices.push_back(vertex);
    }
    else if (str_line.substr(0, 2) == "f ")
    {
      std::vector<int> face;
      std::istringstream iss(str_line.substr(2));
      std::string vertex_data;
      faces_processed++;

      while (iss >> vertex_data)
      {
        // Handle format like "1/2/3" - extract first number only
        std::istringstream viss(vertex_data);
        std::string indexStr;
        if (std::getline(viss, indexStr, '/'))
        {
          int index = std::stoi(indexStr) - 1; // OBJ indices are 1-based
          face.push_back(index);
        }
      }
      // draw lines between the vertices of the face
      for (size_t i = 0; i < face.size(); ++i)
      {
        int v0 = face[i];
        int v1 = face[(i + 1) % face.size()];

        // Transform from object space [-1,1] to screen space [0, width/height]
        int x0 = static_cast<int>((vertices[v0][0] + 1.0f) * frameBuffer.width() / 2.0f);
        int y0 = static_cast<int>((vertices[v0][1] + 1.0f) * frameBuffer.height() / 2.0f);
        int x1 = static_cast<int>((vertices[v1][0] + 1.0f) * frameBuffer.width() / 2.0f);
        int y1 = static_cast<int>((vertices[v1][1] + 1.0f) * frameBuffer.height() / 2.0f);

        line(x0, y0, x1, y1, frameBuffer, lineColor);
      }
    }
  }

  std::cout << "Finished drawing wireframe from " << filename << std::endl;
  std::cout << "Total vertices: " << vertices.size() << std::endl;
  std::cout << "Total faces processed: " << faces_processed << std::endl;
  file.close();
}
