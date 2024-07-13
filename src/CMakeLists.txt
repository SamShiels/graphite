set(SHADER_SOURCES graphite.cpp wgpu_utils.cpp)
set(HEADER_FILES wgpu_utils.h)

add_subdirectory(render)
add_subdirectory(shaders)

# Add these sources to the executable target
target_sources(graphite PRIVATE ${SHADER_SOURCES} ${HEADER_FILES})