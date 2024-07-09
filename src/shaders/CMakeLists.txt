# shaders/CMakeLists.txt

function(embed_shader input_file output_file var_name)
    file(READ ${input_file} shader_code)
    set(shader_content "const char* ${var_name} = R\"(\n${shader_code}\n)\";\n")
    file(APPEND ${output_file} "${shader_content}")
endfunction()

# Clear the file before writing
file(WRITE ${CMAKE_BINARY_DIR}/embedded_shaders.h "")

# Embed shaders
embed_shader(${CMAKE_CURRENT_SOURCE_DIR}/basic.vertex.wgsl ${CMAKE_BINARY_DIR}/embedded_shaders.h vertexShaderCode)
embed_shader(${CMAKE_CURRENT_SOURCE_DIR}/basic.fragment.wgsl ${CMAKE_BINARY_DIR}/embedded_shaders.h fragmentShaderCode)
