set(SHADER_SOURCES graphite.cpp renderObject.cpp transform.cpp)

add_subdirectory(utils)
add_subdirectory(render)
add_subdirectory(shaders)

# Add these sources to the executable target
target_sources(graphite PRIVATE ${SHADER_SOURCES} ${HEADER_FILES})