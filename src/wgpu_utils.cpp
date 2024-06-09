#include "wgpu_utils.h"

#include <future>
#include <webgpu/webgpu_cpp.h>
#include <iostream>
#include "shader/shader.h"

wgpu::Device GetDeviceSync(wgpu::Instance instance) {
  std::promise<wgpu::Device> devicePromise;
  std::future<wgpu::Device> deviceFuture = devicePromise.get_future();

  instance.RequestAdapter(nullptr, [](WGPURequestAdapterStatus status, WGPUAdapter cAdapter, const char* message, void* userData) {
    if (status != WGPURequestAdapterStatus_Success) {
        exit(0);
    }

    wgpu::Adapter adapter = wgpu::Adapter::Acquire(cAdapter);
    adapter.RequestDevice(nullptr, [](WGPURequestDeviceStatus status, WGPUDevice cDevice, const char* message, void* userData) {
        wgpu::Device device = wgpu::Device::Acquire(cDevice);

        device.SetUncapturedErrorCallback([](WGPUErrorType type, const char* message, void* userData) {
        std::cout << "Error: " << type << " - message: " << message;
        }, nullptr);

        std::promise<wgpu::Device>* devicePromise = reinterpret_cast<std::promise<wgpu::Device>*>(userData);
        devicePromise->set_value(device);
    }, userData);
  }, reinterpret_cast<void*>(&devicePromise));

  return deviceFuture.get();
}

wgpu::RenderPipeline CreateRenderPipeline(wgpu::Device device, const char* vertexShaderCode, const char* fragmentShaderCode) {
  wgpu::ShaderModule vertexShaderModule = CreateShaderModule(device, vertexShaderCode);
  wgpu::ShaderModule fragmentShaderModule = CreateShaderModule(device, fragmentShaderCode);

  wgpu::ColorTargetState colorTargetState
  {
    .format = wgpu::TextureFormat::BGRA8Unorm
  };

  wgpu::PrimitiveState primitiveState = {
    .topology = wgpu::PrimitiveTopology::TriangleList,
    .frontFace = wgpu::FrontFace::CCW,
    .cullMode = wgpu::CullMode::None
  };

  wgpu::FragmentState fragmentState =
  {
    .module = fragmentShaderModule,
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
    .vertex = {
      .module = vertexShaderModule
    },
    .primitive = primitiveState,
    .multisample = multisampleState,
    .fragment = &fragmentState
  };

  return device.CreateRenderPipeline(&descriptor);
}