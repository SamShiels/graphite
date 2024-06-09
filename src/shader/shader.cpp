#include <webgpu/webgpu_cpp.h>

wgpu::ShaderModule CreateShaderModule(wgpu::Device device, const char* shaderCode) {
  wgpu::ShaderModuleWGSLDescriptor wgslDescriptor{};

  wgslDescriptor.code = shaderCode;

  wgpu::ShaderModuleDescriptor shaderModuleDescriptor
  {
    .nextInChain = &wgslDescriptor
  };
  
  return device.CreateShaderModule(&shaderModuleDescriptor);
}