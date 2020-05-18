
set(NAME RT2)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_shaders(SHADER_SOURCES)
list(APPEND SOURCES ${SHADER_SOURCES})

add_executable(${NAME} ${SOURCES})

target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${NAME} PRIVATE Vulkan::Vulkan)

target_link_libraries(${NAME} glfw)
target_link_libraries(${NAME} Vulkan::Vulkan)
target_link_libraries(${NAME} VulkanMemoryAllocator)

target_link_libraries(${NAME} Core)
