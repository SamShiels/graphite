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

    void UploadPositions(const void* data, int size);
    void UploadIndices(const void* data, int size);
    void Render();

  private:
    wgpu::Device device;
    wgpu::SwapChain swapChain;
    wgpu::RenderPassDescriptor renderPass;
    wgpu::RenderPipeline pipeline;
    wgpu::Buffer buffer;
    wgpu::Buffer indexBuffer;
    wgpu::BufferUsage bufferUsage;

    uint64_t vertexCount;
    uint64_t indexCount;

    wgpu::ShaderModule CreateShaderModule(const char* shaderCode);
    void CreateRenderPipeline(const char* vertexShaderCode, const char* fragmentShaderCode);
    void CreateBuffer(uint64_t maximumSize);
    void CreateIndexBuffer(uint64_t maximumSize);
};

#endif // RENDERGROUP_H