#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <renderObject.h>
#include <vector>

class Mesh : public RenderObject {
  public:
    Mesh();
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif