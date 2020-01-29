
set(NAME Core)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)
add_library(${NAME} ${SOURCES})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${NAME} PRIVATE Vulkan::Vulkan)

target_link_libraries(${NAME} glfw)
target_link_libraries(${NAME} Vulkan::Vulkan)
