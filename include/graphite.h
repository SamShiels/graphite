#ifndef GRAPHITE_H
#define GRAPHITE_H

#include <webgpu/webgpu_cpp.h>
#include <vector>
#include <glm/glm.hpp>
#include <sprite.h>

struct Scene {
  std::vector<Sprite> sprites;
};

class Internal;

class Graphite {
  public:
    Graphite(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight);
    void Render(const Scene& scene);
    ~Graphite();

  private:
    Internal* internal;
};

#endif