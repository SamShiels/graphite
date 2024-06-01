#include <webgpu/webgpu_cpp.h>

class Renderer {
  public:
    Renderer(wgpu::Instance instance, wgpu::Surface surface, uint32_t windowWidth, uint32_t windowHeight);
    void Render();
    ~Renderer();

  private:
    wgpu::Instance instance;
    wgpu::Device device;
    wgpu::SwapChain swapChain;
    wgpu::RenderPipeline pipeline;
    void CreatePipeline();
    void SetupSwapChain(wgpu::Surface surface, uint32_t windowWidth, uint32_t windowHeight);
};