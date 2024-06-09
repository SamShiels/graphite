set(SHADER_SOURCES shader.cpp)
set(HEADER_FILES shader.h)
# Add these sources to the executable target
target_sources(graphite PRIVATE ${SHADER_SOURCES} ${HEADER_FILES})