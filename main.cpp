#include <GLFW/glfw3.h>
#include <iostream>
#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_glfw.h>
#include <shader.h>
#include <renderer.h>

const uint32_t kWidth = 512;
const uint32_t kHeight = 512;

int main() {
  if (!glfwInit()) {
    return 1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window =
      glfwCreateWindow(kWidth, kHeight, "WebGPU window", nullptr, nullptr);

  Renderer* renderer = new Renderer(window, kWidth, kHeight);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    renderer->Render();
  }

  delete renderer;
  delete window;
}