#include <webgpu/webgpu_cpp.h>

class Renderer {
  public:
    Renderer(GLFWwindow* window, uint32_t windowWidth, uint32_t windowHeight);
    void Render();
    ~Renderer();

  private:
    wgpu::Instance instance;
    wgpu::Device device;
    wgpu::SwapChain swapChain;
    wgpu::Surface surface;
    wgpu::RenderPipeline pipeline;
    void CreatePipeline();
    void SetupSwapChain(uint32_t windowWidth, uint32_t windowHeight);
};