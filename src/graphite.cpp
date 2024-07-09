#include <webgpu/webgpu_glfw.h>
#include <graphite.h>
#include "wgpu_utils.h"
#include <iostream>
#include "renderGroup/renderGroup.h"
#include "embedded_shaders.h"

class Internal {
  public:
    Internal(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight);
    void Render();
    ~Internal();

  private:
    wgpu::Instance instance;
    wgpu::Device device;
    wgpu::SwapChain swapChain;
    wgpu::Surface surface;
    RenderGroup* renderGroup;
    void SetupSwapChain(uint32_t windowWidth, uint32_t windowHeight);
};

Graphite::Graphite(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight) : internal(new Internal(window, windowWidth, windowHeight)) {}

void Graphite::Render() {
  internal->Render();
}

Graphite::~Graphite() {
  delete internal;
}

Internal::Internal(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight) {
  this->instance = wgpu::CreateInstance();
  this->surface = wgpu::glfw::CreateSurfaceForWindow(instance, window);

  std::cout << "Requesting adapter..." << std::endl;
  wgpu::Adapter adapter = requestAdapter(this->instance);

  std::cout << "Requesting device..." << std::endl;
  device = requestDevice(adapter);

  SetupSwapChain(windowWidth, windowHeight);

  // const char* vertexShaderCode = R"(
  //   @vertex
  //   fn main(@location(0) position: vec2<f32>) -> @builtin(position) vec4<f32> {
  //     return vec4<f32>(position, 0.0, 1.0);
  //   }
  // )";

  // const char* fragmentShaderCode = R"(
  //   @fragment
  //   fn main() -> @location(0) vec4<f32> {
  //     return vec4f(1, 0, 0, 1);
  //   }
  // )";

  float frame1Data[] = {0.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f}; // 4 vertices

  this->renderGroup = new RenderGroup(device, swapChain, wgpu::BufferUsage::Vertex, vertexShaderCode, fragmentShaderCode, 1024);
  this->renderGroup->Upload(frame1Data, sizeof(frame1Data));
}

void Internal::Render() {
  this->renderGroup->Render();

  swapChain.Present();
  instance.ProcessEvents();
}

void Internal::SetupSwapChain(uint32_t windowWidth, uint32_t windowHeight) {
  wgpu::SwapChainDescriptor scDesc{
    .usage = wgpu::TextureUsage::RenderAttachment,
    .format = wgpu::TextureFormat::BGRA8Unorm,
    .width = windowWidth,
    .height = windowHeight,
    .presentMode = wgpu::PresentMode::Fifo};
  swapChain = device.CreateSwapChain(surface, &scDesc);
}

Internal::~Internal() {
  
}
