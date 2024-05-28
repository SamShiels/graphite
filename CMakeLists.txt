cmake_minimum_required(VERSION 3.13)  # CMake version check
project(graphite)                      # Create project "graphite"

# Enable C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Gather all source files
set(SOURCES main.cpp)

# Add the executable target
add_executable(graphite ${SOURCES})

add_subdirectory(src)

if(EMSCRIPTEN)
  set_target_properties(graphite PROPERTIES SUFFIX ".html")
  target_link_options(graphite PRIVATE "-sUSE_WEBGPU=1" "-sUSE_GLFW=3")
else()
  set(DAWN_FETCH_DEPENDENCIES ON)
  add_subdirectory("dawn" EXCLUDE_FROM_ALL)
  target_link_libraries(graphite PRIVATE webgpu_cpp webgpu_dawn webgpu_glfw)
endif()

# Debug: Print the gathered source files
message(STATUS "Source files: ${SOURCES}")