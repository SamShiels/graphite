#ifndef BUFFER_H
#define BUFFER_H

#include <webgpu/webgpu_cpp.h>

enum class BufferUsage {
  Vertex,
  Index,
  Uniform
};

class Buffer {
  public: 
    Buffer(wgpu::Device device, const char* label, int maximumSize, BufferUsage usage);
    void Upload(const void* data, int dataSize);
    int GetSize();
    wgpu::Buffer GetWGPUBuffer();
    ~Buffer();

  private:
    wgpu::Buffer buffer;
    wgpu::Device device;
    int elementCount;
    wgpu::BufferUsage ConvertUsage(BufferUsage usage);
};

#endif