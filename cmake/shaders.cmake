cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

function(add_shaders SHADER_FILES_OUT_NAME)
    set(ALL_SHADERS)

    # This will be the output directory for all shaders
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders)

    file(GLOB VERTEX_SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/*.vert)
    foreach(SHADER ${VERTEX_SHADERS})
        set(BUILT_SHADER_NAME "${SHADER}.spv")
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" BUILT_SHADER_NAME ${BUILT_SHADER_NAME})
        add_custom_command(
                OUTPUT ${BUILT_SHADER_NAME}
                COMMAND  glslc ARGS -fshader-stage=vertex ${SHADER} -o ${BUILT_SHADER_NAME}
                MAIN_DEPENDENCY ${SHADER}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        list(APPEND ALL_SHADERS ${BUILT_SHADER_NAME})
    endforeach()

    file(GLOB FRAGMENT_SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/*.frag)
    foreach(SHADER ${FRAGMENT_SHADERS})
        set(BUILT_SHADER_NAME "${SHADER}.spv")
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" BUILT_SHADER_NAME ${BUILT_SHADER_NAME})
        add_custom_command(
                OUTPUT ${BUILT_SHADER_NAME}
                COMMAND glslc ARGS -fshader-stage=fragment ${SHADER} -o ${BUILT_SHADER_NAME}
                MAIN_DEPENDENCY ${SHADER}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        list(APPEND ALL_SHADERS ${BUILT_SHADER_NAME})
    endforeach()
    set(${SHADER_FILES_OUT_NAME} ${ALL_SHADERS} PARENT_SCOPE)
endfunction()
