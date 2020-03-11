set(NAME VulkanMemoryAllocator)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES VMAImplementation.cpp)
add_library(${NAME} ${SOURCES})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/VulkanMemoryAllocator/src/)
target_include_directories(${NAME} PRIVATE Vulkan::Vulkan)

target_link_libraries(${NAME} Vulkan::Vulkan)
target_link_libraries(${NAME} Threads::Threads)
