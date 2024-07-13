#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include "buffer.h"
#include <webgpu/webgpu_cpp.h>

class RenderGroup {
  public:
    RenderGroup(
      wgpu::Device device,
      wgpu::SwapChain swapChain,
      const char* vertexShaderCode,
      const char* fragmentShaderCode,
      uint64_t maximumSize
    );

    ~RenderGroup();

    void UploadPositions(const void* data, int size);
    void UploadIndices(const void* data, int size);
    void Render();

  private:
    wgpu::Device device;
    wgpu::SwapChain swapChain;
    wgpu::RenderPassDescriptor renderPass;
    wgpu::RenderPipeline pipeline;
    Buffer* buffer;
    Buffer* indexBuffer;
    
    uint64_t indexCount;

    wgpu::ShaderModule CreateShaderModule(const char* shaderCode);
    void CreateRenderPipeline(const char* vertexShaderCode, const char* fragmentShaderCode);
};

#endif // RENDERGROUP_H