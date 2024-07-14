#ifndef WGPU_UTILS_H
#define WGPU_UTILS_H

#include <webgpu/webgpu_cpp.h>

wgpu::Adapter requestAdapter(wgpu::Instance instance);
wgpu::Device requestDevice(wgpu::Adapter adapter);

#endif