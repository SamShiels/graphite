set(SHADER_SOURCES wgpu_utils.cpp)
set(HEADER_FILES wgpu_utils.h)
# Add these sources to the executable target
target_sources(graphite PRIVATE ${SHADER_SOURCES} ${HEADER_FILES})