#include "wgpu_utils.h"

#include <future>
#include <webgpu/webgpu_cpp.h>
#include <iostream>
#include "shader/shader.h"
#include <cassert>

wgpu::Adapter requestAdapter(wgpu::Instance instance) {
  struct UserData {
    wgpu::Adapter adapter = nullptr;
    bool requestEnded = false;
  };

  UserData userData;

  auto adapterRequestCallback = [](WGPURequestAdapterStatus status, WGPUAdapter cAdapter, char const * message, void * pUserData) {
    UserData& userData = *reinterpret_cast<UserData*>(pUserData);
    if (status == WGPURequestAdapterStatus::WGPURequestAdapterStatus_Success) {
      wgpu::Adapter adapter = wgpu::Adapter::Acquire(cAdapter);
      userData.adapter = adapter;
    } else {
      std::cout << "Could not obtain WebGPU adaper: " << message << std::endl;
    }

    userData.requestEnded = true;
  };

  instance.RequestAdapter(nullptr, adapterRequestCallback, &userData);
  assert(userData.requestEnded);

  return userData.adapter;
}

wgpu::Device requestDevice(wgpu::Adapter adapter) {
  struct UserData {
    wgpu::Device device = nullptr;
    bool requestEnded = false;
  };

  UserData userData;

  auto deviceRequestCallback = [](WGPURequestDeviceStatus status, WGPUDevice cDevice, const char* message, void * pUserData) {
    UserData& userData = *reinterpret_cast<UserData*>(pUserData);
    if (status == WGPURequestDeviceStatus::WGPURequestDeviceStatus_Success) {
      wgpu::Device device = wgpu::Device::Acquire(cDevice);
      userData.device = device;

      auto errorCallback = [](WGPUErrorType type, const char* message, void* pUserData) {
        std::cout << "Error: " << type << " - message: " << message;
      };

      device.SetUncapturedErrorCallback(errorCallback, nullptr);
    }

    userData.requestEnded = true;
  };

  adapter.RequestDevice(nullptr, deviceRequestCallback, &userData);
  assert(userData.requestEnded);

  return userData.device;
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
      .module = vertexShaderModule,
    },
    .primitive = primitiveState,
    .multisample = multisampleState,
    .fragment = &fragmentState
  };

  wgpu::RenderPipeline pipeline = device.CreateRenderPipeline(&descriptor);
  return pipeline;
}