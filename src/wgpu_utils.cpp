#include "wgpu_utils.h"

#include <future>
#include <webgpu/webgpu_cpp.h>
#include <iostream>

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

wgpu::RenderPipeline CreateRenderPipeline(wgpu::Device device, const char* shaderCode) {
  wgpu::ShaderModuleWGSLDescriptor wgslDesc{};

  wgslDesc.code = shaderCode;

  wgpu::ShaderModuleDescriptor shaderModuleDescriptor
  {
    .nextInChain = &wgslDesc
  };
  
  wgpu::ShaderModule shaderModule = device.CreateShaderModule(&shaderModuleDescriptor);

  wgpu::ColorTargetState colorTargetState
  {
    .format = wgpu::TextureFormat::BGRA8Unorm
  };

  wgpu::FragmentState fragmentState =
  {
    .module = shaderModule,
    .targetCount = 1,
    .targets = &colorTargetState
  };

  wgpu::RenderPipelineDescriptor descriptor
  {
    .vertex = {
      .module = shaderModule
    },
    .fragment = &fragmentState
  };

  return device.CreateRenderPipeline(&descriptor);
}