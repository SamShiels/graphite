#include "buffer.h"
#include <webgpu/webgpu_cpp.h>
#include <iostream>

Buffer::Buffer(wgpu::Device device, const char* label, int maximumSize, BufferUsage usage) {
  this->device = device;

  wgpu::BufferDescriptor bufferDesc = {};
  bufferDesc.size = maximumSize;
  bufferDesc.usage = ConvertUsage(usage) | wgpu::BufferUsage::CopyDst;
  bufferDesc.mappedAtCreation = false;
  bufferDesc.label = label;
  
  this->buffer = device.CreateBuffer(&bufferDesc);
  this->elementCount = 0;
}

wgpu::BufferUsage Buffer::ConvertUsage(BufferUsage usage) {
  switch (usage) {
    case BufferUsage::Vertex:
      return wgpu::BufferUsage::Vertex;
    case BufferUsage::Index:
      return wgpu::BufferUsage::Index;
    case BufferUsage::Uniform:
      return wgpu::BufferUsage::Uniform;
  }
}

void Buffer::Upload(const void* data, int dataSize) {
  if (dataSize > buffer.GetSize()) {
    std::cout << "Too big" << std::endl;
  }

  elementCount = dataSize;

  wgpu::Queue queue = device.GetQueue();
  queue.WriteBuffer(buffer, 0, data, dataSize);
}

int Buffer::GetSize() {
  return elementCount;
}

wgpu::Buffer Buffer::GetWGPUBuffer() {
  return this->buffer;
}

Buffer::~Buffer() {
  if (buffer) {
    buffer.Destroy();
  }
  
  // Set the buffer to null to prevent any accidental use after destruction
  buffer = nullptr;
  
  // Reset other members if needed
  device = nullptr;
  elementCount = 0;
}
