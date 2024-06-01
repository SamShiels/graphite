#include <renderer.h>
#include "wgpu_utils.h"

Renderer::Renderer(wgpu::Instance instance, wgpu::Surface surface, uint32_t windowWidth, uint32_t windowHeight) {
  this->instance = instance;
  device = GetDeviceSync(instance);
  SetupSwapChain(surface, windowWidth, windowHeight);
  CreatePipeline();
}

void Renderer::Render() {
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

void Renderer::SetupSwapChain(wgpu::Surface surface, uint32_t windowWidth, uint32_t windowHeight) {
  wgpu::SwapChainDescriptor scDesc{
    .usage = wgpu::TextureUsage::RenderAttachment,
    .format = wgpu::TextureFormat::BGRA8Unorm,
    .width = windowWidth,
    .height = windowHeight,
    .presentMode = wgpu::PresentMode::Fifo};
  swapChain = device.CreateSwapChain(surface, &scDesc);
}

void Renderer::CreatePipeline() {
  wgpu::ShaderModuleWGSLDescriptor wgslDesc{};

  char shaderCode[] = R"(
    @vertex fn vertexMain(@builtin(vertex_index) i : u32) ->
      @builtin(position) vec4f {
        const pos = array(vec2f(0, 1), vec2f(-1, -1), vec2f(1, -1));
        return vec4f(pos[i], 0, 1);
    }
    @fragment fn fragmentMain() -> @location(0) vec4f {
        return vec4f(1, 0, 0, 1);
    }
  )";

  wgslDesc.code = shaderCode;

  wgpu::ShaderModuleDescriptor shaderModuleDescriptor
  {
    .nextInChain = &wgslDesc
  };
  
  wgpu::ShaderModule shaderModule = device.CreateShaderModule(&shaderModuleDescriptor);

  wgpu::ColorTargetState colorTargetState
  {
    .format = wgpu::TextureFormat::BGRA8Unorm
  };

  wgpu::FragmentState fragmentState =
  {
    .module = shaderModule,
    .targetCount = 1,
    .targets = &colorTargetState
  };

  wgpu::RenderPipelineDescriptor descriptor
  {
    .vertex = {
      .module = shaderModule
    },
    .fragment = &fragmentState
  };

  pipeline = device.CreateRenderPipeline(&descriptor);
}

Renderer::~Renderer() {

}