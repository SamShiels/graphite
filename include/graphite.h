#ifndef GRAPHITE_H
#define GRAPHITE_H

#include <webgpu/webgpu_cpp.h>

class Internal;

class Graphite {
  public:
    Graphite(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight);
    void Render();
    ~Graphite();

  private:
    Internal* internal;
};

#endif