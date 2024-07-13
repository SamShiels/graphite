set(SHADER_SOURCES renderGroup.cpp buffer.cpp)
set(HEADER_FILES renderGroup.h buffer.h)
# Add these sources to the executable target
target_sources(graphite PRIVATE ${SHADER_SOURCES} ${HEADER_FILES})