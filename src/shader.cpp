#include <shader.h>
#include <webgpu/webgpu_cpp.h>

Shader::Shader(wgpu::Device device, const char* code) {
  wgpu::ShaderModuleWGSLDescriptor wgslDesc{};
  wgslDesc.code = code;

  wgpu::ShaderModuleDescriptor shaderModuleDescriptor
  {
    .nextInChain = &wgslDesc
  };
  
  shaderModule = device.CreateShaderModule(&shaderModuleDescriptor);

  wgpu::ColorTargetState colorTargetState
  {
    .format = wgpu::TextureFormat::BGRA8Unorm
  };

  fragmentState =
  {
    .module = shaderModule,
    .targetCount = 1,
    .targets = &colorTargetState
  };
};

wgpu::ShaderModule Shader::GetModule() {
  return shaderModule;
};

wgpu::FragmentState* Shader::GetFragmentState() {
  return &fragmentState;
};

Shader::~Shader() {
  
}