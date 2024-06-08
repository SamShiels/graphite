#ifndef GRAPHITE_H
#define GRAPHITE_H

#include <webgpu/webgpu_cpp.h>

class Graphite {
  public:
    Graphite(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight);
    void Render();
    ~Graphite();

  private:
    wgpu::Instance instance;
    wgpu::Device device;
    wgpu::SwapChain swapChain;
    wgpu::Surface surface;
    wgpu::RenderPipeline pipeline;
    void CreatePipeline();
    void SetupSwapChain(uint32_t windowWidth, uint32_t windowHeight);
};

#endif