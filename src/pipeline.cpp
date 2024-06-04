#include "pipeline.h"
#include <webgpu/webgpu_cpp.h>

Pipeline::Pipeline(wgpu::Device device, const char* shaderCode) {
  wgpu::ShaderModuleWGSLDescriptor wgslDesc{};

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

  this->wgpuPipeline = device.CreateRenderPipeline(&descriptor);
}

wgpu::RenderPipeline Pipeline::GetWGPUPipeline() {
  return wgpuPipeline;
}