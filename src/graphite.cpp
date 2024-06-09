#include <webgpu/webgpu_glfw.h>
#include <graphite.h>
#include "wgpu_utils.h"

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
    wgpu::RenderPipeline pipeline;
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
  device = GetDeviceSync(instance);
  SetupSwapChain(windowWidth, windowHeight);

  const char* vertexShaderCode = R"(
    @vertex fn vertexMain(@builtin(vertex_index) i : u32) ->
      @builtin(position) vec4f {
        const pos = array(vec2f(0, 1), vec2f(-1, -1), vec2f(1, -1));
        return vec4f(pos[i], 0, 1);
    }
  )";

  const char* fragmentShaderCode = R"(
    @fragment fn fragmentMain() -> @location(0) vec4f {
        return vec4f(1, 0, 0, 1);
    }
  )";

  this->pipeline = CreateRenderPipeline(device, vertexShaderCode, fragmentShaderCode);
}

void Internal::Render() {
  wgpu::RenderPassColorAttachment attachment
  {
    .view = swapChain.GetCurrentTextureView(),
    .loadOp = wgpu::LoadOp::Clear,
    .storeOp = wgpu::StoreOp::Store
  };

  wgpu::RenderPassDescriptor renderpass
  {
    .colorAttachmentCount = 1,
    .colorAttachments = &attachment
  };

  wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
  wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
  pass.SetPipeline(pipeline);
  pass.Draw(3);
  pass.End();
  wgpu::CommandBuffer commands = encoder.Finish();
  device.GetQueue().Submit(1, &commands);

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
