set(SHADER_SOURCES graphite.cpp pipeline.cpp wgpu_utils.cpp)
set(HEADER_FILES wgpu_utils.h pipeline.h)
# Add these sources to the executable target
target_sources(graphite PRIVATE ${SHADER_SOURCES} ${HEADER_FILES})