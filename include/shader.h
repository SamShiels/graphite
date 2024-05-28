#include <webgpu/webgpu_cpp.h>

class Shader {
  public:
    Shader(wgpu::Device device, const char* code);
    wgpu::ShaderModule GetModule();
    wgpu::FragmentState* GetFragmentState();
    ~Shader();

  private:
    wgpu::ShaderModule shaderModule;
    wgpu::FragmentState fragmentState;
};