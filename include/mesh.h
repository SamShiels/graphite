#ifndef MESH_H
#define MESH_H

#include <renderObject.h>
#include <vector>

class Mesh : public RenderObject {
  public:
    Mesh();

    void setVertices(const std::vector<float>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);

  protected:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif