#include <GLFW/glfw3.h>
#include <iostream>
#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_glfw.h>
#include <graphite.h>
#include <sprite.h>

const uint32_t kWidth = 800;
const uint32_t kHeight = 800;

int main() {
  if (!glfwInit()) {
    return 1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window =
      glfwCreateWindow(kWidth, kHeight, "WebGPU window", nullptr, nullptr);

  Graphite* graphite = new Graphite(window, kWidth, kHeight);

  Scene scene;

  scene.sprites = {};

  Sprite sprite = Sprite(0.2, 0.2);
  scene.sprites.push_back(sprite);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    graphite->Render(scene);
  }

  delete graphite;
}