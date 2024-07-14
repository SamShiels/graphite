#include <mesh.h>

Mesh::Mesh() : RenderObject() {
}

void Mesh::setVertices(const std::vector<float>& vertices) {
  this->vertices = vertices;
}

void Mesh::setIndices(const std::vector<unsigned int>& indices) {
  this->indices = indices; 
}
