#include <webgpu/webgpu_cpp.h>
#include "renderGroup.h"
#include <iostream>

RenderGroup::RenderGroup(
    wgpu::Device device, 
    wgpu::SwapChain swapChain, 
    wgpu::BufferUsage bufferUsage, 
    const char* vertexShaderCode, 
    const char* fragmentShaderCode,
    uint64_t maximumSize
  ) {
  this->device = device;
  this->swapChain = swapChain;
  this->bufferUsage = bufferUsage;

  CreateBuffer(maximumSize);
  CreateIndexBuffer(maximumSize);
  CreateRenderPipeline(vertexShaderCode, fragmentShaderCode);
}

wgpu::ShaderModule RenderGroup::CreateShaderModule(const char* shaderCode) {
  wgpu::ShaderModuleWGSLDescriptor wgslDescriptor{};

  wgslDescriptor.code = shaderCode;

  wgpu::ShaderModuleDescriptor shaderModuleDescriptor
  {
    .nextInChain = &wgslDescriptor
  };
  
  return device.CreateShaderModule(&shaderModuleDescriptor);
}

void RenderGroup::CreateRenderPipeline(const char* vertexShaderCode, const char* fragmentShaderCode) {
  wgpu::ShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode);
  wgpu::ShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderCode);

  wgpu::PipelineLayoutDescriptor layoutDesc = {
    .bindGroupLayoutCount = 0,
    .bindGroupLayouts = nullptr
  };

  wgpu::PipelineLayout pipelineLayout = device.CreatePipelineLayout(&layoutDesc);

  wgpu::ColorTargetState colorTargetState
  {
    .format = wgpu::TextureFormat::BGRA8Unorm
  };

  wgpu::PrimitiveState primitiveState = {
    .topology = wgpu::PrimitiveTopology::TriangleList,
    .frontFace = wgpu::FrontFace::CW,
    .cullMode = wgpu::CullMode::None
  };

  wgpu::VertexAttribute vertexAttributes[1] = {};
  vertexAttributes[0].format = wgpu::VertexFormat::Float32x2;
  vertexAttributes[0].offset = 0;
  vertexAttributes[0].shaderLocation = 0;

  wgpu::VertexBufferLayout vertexBufferLayout = {
    .arrayStride = 2 * sizeof(float),
    .attributeCount = 1,
    .attributes = vertexAttributes
  };

  wgpu::VertexState vertexState =
  {
    .module = vertexShaderModule,
    .entryPoint = "main",
    .bufferCount = 1,
    .buffers = &vertexBufferLayout
  };

  wgpu::FragmentState fragmentState =
  {
    .module = fragmentShaderModule,
    .entryPoint = "main",
    .targetCount = 1,
    .targets = &colorTargetState
  };

  wgpu::MultisampleState multisampleState =
  {
    .count = 1,
    .mask = 0xFFFFFFFF,
    .alphaToCoverageEnabled = false
  };

  wgpu::RenderPipelineDescriptor descriptor
  {
    .vertex = vertexState,
    .primitive = primitiveState,
    .multisample = multisampleState,
    .fragment = &fragmentState
  };

  this->pipeline = device.CreateRenderPipeline(&descriptor);
}

void RenderGroup::CreateBuffer(uint64_t maximumSize) {
  wgpu::BufferDescriptor bufferDesc = {};
  bufferDesc.size = maximumSize * sizeof(float);
  bufferDesc.usage = this->bufferUsage | wgpu::BufferUsage::CopyDst;

  buffer = device.CreateBuffer(&bufferDesc);
}

void RenderGroup::CreateIndexBuffer(uint64_t maximumSize) {
  wgpu::BufferDescriptor bufferDesc = {};
  bufferDesc.size = maximumSize * sizeof(uint16_t);
  bufferDesc.label = "Index buffer";
  bufferDesc.usage = wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst;

  indexBuffer = device.CreateBuffer(&bufferDesc);
}

void RenderGroup::UploadPositions(const void* data, uint16_t size) {
  if (size > buffer.GetSize()) {
    std::cout << "Too big!!" << std::endl;
  }

  vertexCount = size / (2 * sizeof(float));

  wgpu::Queue queue = device.GetQueue();
  queue.WriteBuffer(buffer, 0, data, size);
}

void RenderGroup::UploadIndices(const void* data, uint16_t size) {
  if (size > indexBuffer.GetSize()) {
    std::cout << "Too big!!" << std::endl;
  }

  indexCount = size / sizeof(uint16_t);

  wgpu::Queue queue = device.GetQueue();
  queue.WriteBuffer(indexBuffer, 0, data, size);
}

void RenderGroup::Render() {
  wgpu::RenderPassColorAttachment attachment
  {
    .view = swapChain.GetCurrentTextureView(),
    .loadOp = wgpu::LoadOp::Clear,
    .storeOp = wgpu::StoreOp::Store
  };

  wgpu::RenderPassDescriptor renderpass
  {
    .colorAttachmentCount = 1,
    .colorAttachments = &attachment
  };

  wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
  wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
  pass.SetPipeline(pipeline);
  pass.SetVertexBuffer(0, buffer);
  pass.SetIndexBuffer(indexBuffer, wgpu::IndexFormat::Uint16);
  pass.DrawIndexed(indexCount);
  pass.End();
  wgpu::CommandBuffer commands = encoder.Finish();
  device.GetQueue().Submit(1, &commands);
}