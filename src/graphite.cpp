#include <webgpu/webgpu_glfw.h>
#include <graphite.h>
#include "utils/wgpu_utils.h"
#include <iostream>
#include "render/renderGroup.h"
#include "embedded_shaders.h"

class Internal {
  public:
    Internal(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight);
    void Render(const Scene& scene);
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

void Graphite::Render(const Scene& scene) {
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
  this->renderGroup = new RenderGroup(device, swapChain, vertexShaderCode, fragmentShaderCode, 1024);
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

void Internal::Render(const Scene& scene) {
  std::vector<float> vertices;
  std::vector<int> indices;

  vertices.reserve(scene.sprites.size() * 8);  // 4 vertices per sprite
  indices.reserve(scene.sprites.size() * 6);   // 6 indices per sprite

  int indexBase = 0;

  for (int i = 0; i < scene.sprites.size(); i++) {
    Sprite sprite = scene.sprites[i];

    glm::vec3 position = sprite.transform.getPosition();

    vertices.push_back(position.x);
    vertices.push_back(position.y);

    vertices.push_back(position.x + sprite.width);
    vertices.push_back(position.y);

    vertices.push_back(position.x + sprite.width);
    vertices.push_back(position.y + sprite.height);

    vertices.push_back(position.x);
    vertices.push_back(position.y + sprite.height);

    indices.push_back(indexBase);
    indices.push_back(indexBase + 1);
    indices.push_back(indexBase + 2);
    indices.push_back(indexBase);
    indices.push_back(indexBase + 2);
    indices.push_back(indexBase + 3);

    indexBase += 4;
  }

  int vertexSize = vertices.size() * sizeof(float);
  int indexSize = indices.size() * sizeof(int);

  this->renderGroup->UploadPositions(vertices.data(), vertexSize);
  this->renderGroup->UploadIndices(indices.data(), indexSize);
  this->renderGroup->Render();

  swapChain.Present();
  instance.ProcessEvents();
}

Internal::~Internal() {
  delete renderGroup;
}
