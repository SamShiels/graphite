#include <webgpu/webgpu_glfw.h>
#include <graphite.h>
#include "wgpu_utils.h"
#include <iostream>
#include "renderGroup/renderGroup.h"
#include "embedded_shaders.h"

class Internal {
  public:
    Internal(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight);
    void Render(Scene scene);
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

void Graphite::Render(Scene scene) {
  internal->Render(scene);
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

  // float frame1Data[] = {0.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f}; // 4 vertices

  this->renderGroup = new RenderGroup(device, swapChain, wgpu::BufferUsage::Vertex, vertexShaderCode, fragmentShaderCode, 1024);
  // this->renderGroup->Upload(frame1Data, sizeof(frame1Data));
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

void Internal::Render(Scene scene) {
  float* vertices = new float[scene.sprites.size() * 8];
  uint16_t* indices = new uint16_t[scene.sprites.size() * 6];

  uint16_t indexBase = 0;

  for (uint32_t i = 0; i < scene.sprites.size(); i++) {
    Sprite sprite = scene.sprites[i];

    vertices[i]     = sprite.position.x;
    vertices[i + 1] = sprite.position.y;

    vertices[i + 2] = sprite.position.x + sprite.width;
    vertices[i + 3] = sprite.position.y;

    vertices[i + 4] = sprite.position.x + sprite.width;
    vertices[i + 5] = sprite.position.y + sprite.height;

    vertices[i + 6] = sprite.position.x;
    vertices[i + 7] = sprite.position.y + sprite.height;

    indices[i]     = indexBase;
    indices[i + 1] = indexBase + 1;
    indices[i + 2] = indexBase + 2;
    indices[i + 3] = indexBase + 2;
    indices[i + 4] = indexBase + 1;
    indices[i + 5] = indexBase + 3;

    indexBase += 4;
  }

  float frame1Data[] = {0.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f}; // 4 vertices
  int frame1DataI[] = {0, 1, 2}; // 4 vertices

  uint64_t vertexSize = sizeof(frame1Data);
  uint64_t indexSize = sizeof(frame1DataI);

  this->renderGroup->UploadPositions(frame1Data, vertexSize);
  this->renderGroup->UploadIndices(frame1DataI, indexSize);
  this->renderGroup->Render();

  swapChain.Present();
  instance.ProcessEvents();
}

Internal::~Internal() {
  
}
