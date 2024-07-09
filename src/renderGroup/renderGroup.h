#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include <webgpu/webgpu_cpp.h>

class RenderGroup {
  public:
    RenderGroup(
      wgpu::Device device,
      wgpu::SwapChain swapChain,
      wgpu::BufferUsage bufferUsage,
      const char* vertexShaderCode,
      const char* fragmentShaderCode,
      uint64_t maximumSize
    );

    ~RenderGroup();

    void Upload(const void* data, uint64_t size);
    void Render();

  private:
    wgpu::Device device;
    wgpu::SwapChain swapChain;
    wgpu::RenderPassDescriptor renderPass;
    wgpu::RenderPipeline pipeline;
    wgpu::Buffer buffer;
    wgpu::BufferUsage bufferUsage;

    uint64_t vertextCount;

    wgpu::ShaderModule CreateShaderModule(const char* shaderCode);
    void CreateRenderPipeline(const char* vertexShaderCode, const char* fragmentShaderCode);
    void CreateBuffer(uint64_t maximumSize);
};

#endif // RENDERGROUP_H