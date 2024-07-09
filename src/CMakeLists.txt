set(SHADER_SOURCES graphite.cpp wgpu_utils.cpp)
set(HEADER_FILES wgpu_utils.h)

add_subdirectory(shaders)
add_subdirectory(renderGroup)

# Add these sources to the executable target
target_sources(graphite PRIVATE ${SHADER_SOURCES} ${HEADER_FILES})