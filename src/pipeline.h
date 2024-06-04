#include <webgpu/webgpu_cpp.h>

class Pipeline {
  public:
    Pipeline(wgpu::Device device, const char* shaderCode);
    wgpu::RenderPipeline GetWGPUPipeline();

  private:
    wgpu::RenderPipeline wgpuPipeline;
};