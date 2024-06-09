#ifndef WGPU_UTILS_H
#define WGPU_UTILS_H

#include <webgpu/webgpu_cpp.h>

wgpu::Device GetDeviceSync(wgpu::Instance instance);
wgpu::RenderPipeline CreateRenderPipeline(wgpu::Device device, const char* vertexShaderCode, const char* fragmentShaderCode);

#endif